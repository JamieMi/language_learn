#include "stdafx.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "backend.h"

Phrase::Phrase(const string& phrase, const string& translation, const time_t& enteredTime, const size_t milestone)
{
	m_phrase = phrase;
	m_translation = translation;
	m_enteredTime = enteredTime;
	m_mileStone = milestone;

	m_priority = 0;
}

Phrase::~Phrase()
{
	for (std::vector<Test*>::iterator itr = history.begin(); itr != history.end(); ++itr)
	{
		delete *itr;
	}
}

BackEnd::BackEnd()
{
	m_batchItr = m_batch.end();

	size_t milestones[] = {
		300,              // 5 minutes
		900,              // 15 minutes
		14400,            // 4 hours
		3600 * 24,        // 1 day
		3600 * 24 * 2,    // 2 days
		3600 * 24 * 3,    // 3 days
		3600 * 24 * 4,    // 4 days
		3600 * 24 * 7,    // 7 days
		3600 * 24 * 10,   // 10 days
		3600 * 24 * 16,   // 16 days
		3600 * 24 * 32,   // 32 days
		3600 * 24 * 80,   // 80 days
		3600 * 24 * 130,  // 130 days
		3600 * 24 * 180,  // 180 days
		3600 * 24 * 270,  // 270 days
		3600 * 24 * 365,  // 1 year
		3600 * 24 * 730,  // 2 years
		3600 * 24 * 1095, // 3 years
		3600 * 24 * 1460, // 4 years
		3600 * 24 * 1825, // 5 years
		0xFFFFFFFF        // forever
	};

	for (int i = 0; i < sizeof(milestones) / sizeof(int); ++i)
	{
		Milestone* pMileStone = new Milestone(milestones[i]);
		m_mileStones.push_back(pMileStone);
	}

	m_bReversed = false;
}

int Phrase::calculatePriority()
{
	//bool bDue = false;
	int dueDays = -1;
	m_priority = 0;
	time_t now = time(0);
	time_t sinceEntered = now - m_enteredTime;
	if (history.size() != 0)
	{
		dueDays = (m_mileStone - sinceEntered) / (24 * 60 * 60) + 1;

		if (sinceEntered > m_mileStone)
		{
			//bDue = true;
			dueDays = 0;
			if (rand() % 100 < 35)  m_priority += 11; // this it to prevent it hammering the same BATCH_SIZE items to completion
			
			auto last = history.back();
			time_t sinceLast = now - (*last).getTime();
			// crude way to ensure that the more we have missed a milestone, the higher the priority
			while (sinceLast > m_mileStone)
			{
				sinceLast /= 2;
				if (rand() % 100 < 35) m_priority += 11;
			}
		}
		else
		{
			m_priority = 2;
		}

		int failedCount = 0;
		for (auto x : history)
		{
			if (!x->getSuccess())
			{
				failedCount++;
			}
		}
		if (failedCount > 4) failedCount = 4;
		m_priority += failedCount;
	}
	else
	{
		if (sinceEntered > m_mileStone)
		{
			dueDays = 0;
			m_priority = 10;
		}
	}
	return dueDays;
}

const int BATCH_SIZE = 6;

void BackEnd::openDueFile()
{
	m_dueItemsFile.open(_T("C:\\temp\\dueitems.txt", std::ios::app | std::ofstream::out | std::ofstream::app));
}

void BackEnd::saveDueFile()
{
	m_dueItemsFile << m_ss.rdbuf();
	m_dueItemsFile.flush();
	m_dueItemsFile.close();
}

bool BackEnd::outputDueItem()
{
	std::ofstream dueItemsFile;

	m_ss << (*m_batchItr)->getTranslation();
	m_ss << '|';
	m_ss << (*m_batchItr)->getPhrase();
	m_ss << endl;
	return (m_stats.dueVocabularySize > 10);
}

CString BackEnd::saveFile()
{
	// Because my lamptop is being an arse and disconnecting the USB the moment
	// it sees it being actively written to, we need to save a copy locally as backup
	// we could deffo refactor this though

	CString szReturn(_T(""));

	// Do backup
	std::ofstream backupFile;
	backupFile.open(_T("C:\\temp\\backuplanguagefile.txt"));

	if (backupFile)
	{
		std::stringstream ss;
		for (vector<Phrase*>::iterator itr = phraseList.begin(); itr != phraseList.end(); ++itr)
		{
			string phrase = "";
			ss << (*itr)->getTranslation();
			ss << '|';
			ss << (*itr)->getPhrase();

			// now save the milestone
			ss << '|';
			ss << (*itr)->getMilestone();

			// now save the creation date - unix time will do OK
			ss << '|';
			ss << (*itr)->getEnteredTime();

			// now save the history - unix time will do OK

			for (auto& test : (*itr)->history)
			{
				ss << '|';
				ss << (*test).getTime();
				ss << '|';
				ss << (*test).getSuccess();
			}

			// terminate
			ss << endl;
		}
		backupFile << ss.rdbuf();

	}
	else
	{
		std::perror("Backup saving failed");
		szReturn = _T("Backup saving failed");
	}

	// Do the normal file
	std::ofstream outFile;
	outFile.open(m_filePath.c_str());

	if (outFile)
	{
		std::stringstream ss;
		for (vector<Phrase*>::iterator itr = phraseList.begin(); itr != phraseList.end(); ++itr)
		{
			string phrase = "";
			ss << (*itr)->getTranslation();
			ss << '|';
			ss << (*itr)->getPhrase();

			// now save the milestone
			ss << '|';
			ss << (*itr)->getMilestone();

			// now save the creation date - unix time will do OK
			ss << '|';
			ss << (*itr)->getEnteredTime();

			// now save the history - unix time will do OK

			for (auto& test : (*itr)->history)
			{
				ss << '|';
				ss << (*test).getTime();
				ss << '|';
				ss << (*test).getSuccess();
			}

			// terminate
			ss << endl;
		}
		outFile << ss.rdbuf();

	}
	else
	{
		std::perror("File saving failed");
		szReturn = _T("File saving failed - check backup in C:\\temp");
	}
	return szReturn;
}

void BackEnd::readFile()
{
	std::string phraseEnglish;
	std::string phraseOther;

	std::ifstream inFile(m_filePath.c_str());

	std::string line;
	while (std::getline(inFile, line))
	{
		line.erase(std::remove(line.begin(), line.end(), 0x09), line.end());
		size_t index = line.find('|');
		phraseOther = line.substr(0, index);
		line.erase(0, index + 1);
		index = line.find('|');
		time_t enteredTime = time(0);
		size_t milestoneAchieved = 0;
		Phrase* phr;

		// if we have a third param:
		if (index != std::string::npos)
		{
			// then we have a history
			phraseEnglish = line.substr(0, index);
			line.erase(0, index + 1);

			// get the milestone achieved (in s, so that if we change milestones we can still work out what other milestone it might correspond to)
			index = line.find('|');
			if (index != std::string::npos)
			{
				// neat method to unpack numeral types:
				istringstream(line.substr(0, index)) >> milestoneAchieved;
				line.erase(0, index + 1);
			}
			// (this is a prime candidate for refactoring - only the parameter changes between this and a few other occurrences)

			// now get the entered time:
			index = line.find('|');
			if (index != std::string::npos)
			{
				istringstream(line.substr(0, index)) >> enteredTime;
				line.erase(0, index + 1);
			}
			else
			{
				istringstream(line) >> enteredTime;
			}

			phr = new Phrase(phraseEnglish, phraseOther, enteredTime, milestoneAchieved);
			phraseList.push_back(phr);

			// now the history, if there is one
			index = line.find('|');
			if (index != std::string::npos) m_stats.activeVocabularySize++;
			while (index != std::string::npos)
			{
				time_t testTime;
				istringstream(line.substr(0, index)) >> testTime;
				line.erase(0, index + 1);

				index = line.find('|');
				bool bSuccess;
				if (index != std::string::npos)
				{
					istringstream(line.substr(0, index)) >> bSuccess;
					line.erase(0, index + 1);
					index = line.find('|');
				}
				else
				{
					istringstream(line) >> bSuccess;
				}

				Test* pTest = new Test(bSuccess, testTime);

				phr->history.push_back(pTest);

				if (bSuccess) {
					m_stats.successes++;
				}
				else {
					m_stats.failures++;
				}
			}
		}
		else // all that remains
		{
			phraseEnglish = line;

			phr = new Phrase(phraseEnglish, phraseOther, enteredTime, milestoneAchieved);
			phraseList.push_back(phr);
		}

		m_stats.vocabularySize++;
	}
}


void BackEnd::readReferenceFile()
{
	// text is structured like the vocabulary file, although not every line will be vocab

	std::string phraseEnglish;
	std::string phraseOther;

	string m_referenceFilePath = "C:\\temp\\German words to do.txt";
	std::ifstream inFile(m_referenceFilePath.c_str());

	std::string line;
	while (std::getline(inFile, line))
	{
		size_t index = line.find('|');

		if (index == std::string::npos)
		{
			if (line.empty() || line.at(0)=='=')
			{
				continue;
			}
			else
			{ 
				// we'll assume that a line formatted without a translation might be in the other language without a translation
				phraseOther = line;
				phraseEnglish = "";
			}
		}
		else
		{
			phraseOther = line.substr(0, index);
			line.erase(0, index + 1);
			phraseEnglish = line;
		}

		Phrase* phr = new Phrase(phraseEnglish, phraseOther); // we're only interested in using the text here - time and status don't matter
		referenceList.push_back(phr);
	}

	// Now order these alphabetically. This may help when we deal with duplicates.
	std::sort(referenceList.begin(), referenceList.end(), alphTranslationCmp);

}


std::size_t BackEnd::lookupTranslation(Phrase* ourPhrase, Phrase* referenceListPhrase)
{
	std::size_t count = 0;
	
	// not the optimum way to search, and we aren't making use of our ordering yet
	for (auto& x : referenceList)
	{
		if (x->getTranslation() == ourPhrase->getTranslation().c_str())
		{
			count++;
			if (x->getPhrase() == ourPhrase->getPhrase())
			{
				// everything matches (at least for a first match)
				
			}
			else
			{
				// return the alternative English that we have found:
				referenceListPhrase->setPhrase(x->getPhrase());
			}
		}
		else // translation differs
		{
			if (x->getPhrase() == ourPhrase->getPhrase())
			{
				count++;
				// return the alternative translation that we have found:
				referenceListPhrase->setTranslation(x->getTranslation());
			}
			else
			{
				// nope - just not a match of any kind
			}
		}
	}
	return count;
}

void BackEnd::logTest(bool bSuccess)
{
	time_t timeTested(time(0));
	Test* pTest = new Test(bSuccess, timeTested);
	(*m_batchItr)->history.push_back(pTest);

	// have we hit a milestone?
	// For the moment the rule is: we have to pass a test, to achieve the milestone
	if (bSuccess)
	{
		m_stats.successes++;
		m_stats.successesThisSession++;
		time_t timeSinceCreation = timeTested - (*m_batchItr)->getEnteredTime();
		if (timeSinceCreation > (*m_batchItr)->getMilestone())
		{
			// get the next milestone:
			for (auto p : m_mileStones)
			{
				if (p->periodSeconds > (*m_batchItr)->getMilestone())
				{
					// we've found a milestone bigger than the current one
					(*m_batchItr)->setMilestone(p->periodSeconds);
					break;
				}
			}
		}
	}
	else
	{
		m_stats.failures++;
		m_stats.failuresThisSession++;
	}

}

void BackEnd::getSelection()
{
	if (m_batchItr != m_batch.end()) m_batchItr++;
	if (m_batchItr == m_batch.end())
	{
		// get the next BATCH_SIZE items to work through

		m_batch.clear();
		std::vector<Phrase*> prioritisedVector;// temp vector for ordering
		if (phraseList.size() < BATCH_SIZE)
		{
			return;
		}

		// 1. establish priority of every item
		for (size_t iPreviewDays = 0;  iPreviewDays < NUM_PREVIEW_DUE_DAYS; ++iPreviewDays)
		{
			m_stats.dueByDay[iPreviewDays] = 0;
		}

		m_stats.dueVocabularySize = 0;
		for (auto& x : phraseList)
		{
			int dueDays = x->calculatePriority();
			if (dueDays == 0)
			{
				m_stats.dueVocabularySize++;
			}
			if (dueDays < NUM_PREVIEW_DUE_DAYS)
			{
				m_stats.dueByDay[dueDays]++;
			}
			
			prioritisedVector.push_back(x);
		}

		// 2. order these
		std::sort(prioritisedVector.begin(), prioritisedVector.end(), priorityCmp);

		// 3. add top 10 to our vector
		auto pV = prioritisedVector.begin();
		for (int i = 0; i < BATCH_SIZE; ++i)
		{
			m_batch.push_back(*pV);
			pV++;
		}

		m_batchItr = m_batch.begin();
	}
}

BackEnd::~BackEnd()
{
	for (std::vector<Phrase*>::iterator itr = phraseList.begin(); itr != phraseList.end(); ++itr)
	{
		delete *itr;
	}
	phraseList.clear();
	for (std::vector<Milestone*>::iterator itr = m_mileStones.begin(); itr != m_mileStones.end(); ++itr)
	{
		delete *itr;
	}
	m_mileStones.erase(m_mileStones.begin(), m_mileStones.end());
	m_mileStones.clear();
}

void BackEnd::deletePhrase()
{
	// delete just from the batch, for the moment

	Phrase* pPhrase = *m_batchItr;

	std::string phrase = (*m_batchItr)->getPhrase();

	int index = m_batchItr - m_batch.begin();
	std::vector<Phrase*>::iterator next = m_batchItr;
	next++;
	if (next == m_batch.end())
	{
		m_batch.erase(m_batchItr);
		m_batchItr = m_batch.begin();
	}
	else
	{
		m_batch.erase(m_batchItr);
		m_batchItr = m_batch.begin() + index;
	}

	// TO DO: these leave us with the wrong entry, for one instant, before recovering

	// now delete in the main vector

	for (std::vector<Phrase*>::iterator itr = phraseList.begin(); itr != phraseList.end(); ++itr)
	{
		if ((*itr) == pPhrase)
		{
			// delete
			phraseList.erase(itr);
			break;
		}
	}
}

void BackEnd::updatePhrase(Phrase newPhrase)
{
	Phrase* pPhrase = *m_batchItr;

	if (newPhrase.getPhrase() != "")
	{
		pPhrase->setPhrase(newPhrase.getPhrase());
	}
	if (newPhrase.getTranslation() != "")
	{
		pPhrase->setTranslation(newPhrase.getTranslation());
	}
}

void BackEnd::reverse()
{
	m_bReversed = !m_bReversed;
}

void BackEnd::equalise()
{
	for (std::vector<Phrase*>::iterator itr = phraseList.begin(); itr != phraseList.end(); ++itr)
	{
		int adjustment = (60 * 60 * 24) * (rand() % 14);
		(*itr)->setEnteredTime((*itr)->getEnteredTime() - adjustment);
	}
}

bool priorityCmp(Phrase* one, Phrase* two)
{
	return (one->getPriority() >
		two->getPriority());
}

bool alphTranslationCmp(Phrase* one, Phrase* two)
{
	return (one->getTranslation() >
		two->getTranslation());
}






