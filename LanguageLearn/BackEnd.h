#pragma once


#include <string>
#include <vector>
#include "sstream"
#include <iostream>
#include <fstream>

using namespace std;
typedef std::size_t isize;

#ifndef LL_BACKEND
#define LL_BACKEND

const isize NUM_PREVIEW_DUE_DAYS = 7;

class Test
{
public:
	Test(bool success, const time_t& t )
	{
		dateTime = t;
		bSuccess = success;
	}
	time_t getTime() {
		return dateTime;
	}
	bool getSuccess() {
		return  bSuccess;
	}
private:
	time_t dateTime;
	bool bSuccess;
};




class Phrase
{
public:
	Phrase(const string& phrase, const string& translation, const time_t& enteredTime = 0, isize milestoneAchieved = 0);
	~Phrase();
	string getPhrase() {
		//if (!m_bReversed){
			return m_phrase;
			//}
			//else{
			//	return m_translation;
			//}
	}
	string getTranslation() {
		//if (!m_bReversed){
			return m_translation;
			//}
		//else{
		//	return m_phrase;
		//}
	}

	void setPhrase(string phrase) {
		m_phrase = phrase;
	}
	void setTranslation(string translation) {
		m_translation = translation;
	}

	int getPriority() {
		return m_priority;
	}

	int calculatePriority();

	std::size_t getMilestone() const {
		return m_mileStone;
	}
	void setMilestone(const std::size_t milestone ) {
		m_mileStone = milestone;
	}

	time_t getEnteredTime()
	{
		return m_enteredTime;
	}

	void setEnteredTime(time_t newTime)
	{
		m_enteredTime = newTime;
	}

private:
	vector<string> m_tags;
	string m_phrase;
	string m_translation;
	time_t m_enteredTime;
	int m_priority;
	isize m_mileStone;// #s, not an index. Next milestone

	//string serialise();
	void read(string sRecord);
public:
	vector<Test*> history;// TO DO: make private
};

class Milestone
{
public:
	Milestone(std::size_t milestone = 0)
	{
		periodSeconds = milestone;
	}
	isize periodSeconds;
};

class Statistics
{
public:
	Statistics()
	{
		vocabularySize = 0;
		activeVocabularySize = 0;
		dueVocabularySize = 0;
		successes = 0;
		failures = 0;
		successesThisSession = 0;
		failuresThisSession = 0;
		for (int i = 0; i < NUM_PREVIEW_DUE_DAYS; ++i)
		{
			dueByDay.push_back(0);
		}
	}
	isize vocabularySize;
	isize activeVocabularySize;
	isize dueVocabularySize;
	isize successes;
	isize failures;
	isize successesThisSession;
	isize failuresThisSession;
	vector<isize> dueByDay;
};

bool priorityCmp(Phrase* one, Phrase* two);
bool alphTranslationCmp(Phrase* one, Phrase* two);


class BackEnd
{
public:
	BackEnd();
	~BackEnd();
	void getSelection();
	void readFile();
	void readReferenceFile();
	std::size_t lookupTranslation(Phrase* ourPhrase, Phrase* referenceListPhrase);
	CString saveFile();
	void deletePhrase();
	void updatePhrase(Phrase newPhrase);
	bool outputDueItem();
	void logTest(bool bSuccess);
	vector<Phrase*> phraseList;
	vector<Phrase*> referenceList;
	Statistics m_stats;
	std::string m_filePath;
	std::string m_referenceFilePath;
	std::vector<Phrase*> m_batch;
	std::vector<Phrase*>::iterator m_batchItr;
	void reverse();
	void equalise();
	void openDueFile();
	void saveDueFile();
private:
	std::vector<Milestone*> m_mileStones;
	bool m_bReversed;
	std::stringstream m_ss;
	std::ofstream m_dueItemsFile;
};

#endif LL_BACKEND



