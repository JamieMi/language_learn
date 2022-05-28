#pragma once


#include <string>
#include <vector>

using namespace std;

#ifndef LL_BACKEND
#define LL_BACKEND

const size_t NUM_PREVIEW_DUE_DAYS = 7;


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
	Phrase(const string& phrase, const string& translation, const time_t& enteredTime, const size_t milestoneAchieved = 0);
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
	int getPriority() {
		return m_priority;
	}
	int calculatePriority();

	size_t getMilestone() const {
		return m_mileStone;
	}
	void setMilestone(const size_t milestone ) {
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
	size_t m_mileStone;// #s, not an index. Next milestone

	//string serialise();
	void read(string sRecord);
public:
	vector<Test*> history;// TO DO: make private
};

class Milestone
{
public:
	Milestone(size_t milestone = 0)
	{
		periodSeconds = milestone;
	}
	size_t periodSeconds;
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
	size_t vocabularySize;
	size_t activeVocabularySize;
	size_t dueVocabularySize;
	size_t successes;
	size_t failures;
	size_t successesThisSession;
	size_t failuresThisSession;
	vector<size_t> dueByDay;
};

bool priorityCmp(Phrase* one, Phrase* two);

class BackEnd
{
public:
	BackEnd();
	~BackEnd();
	void getSelection();
	void readFile();
	CString saveFile();
	void deletePhrase();
	void logTest(bool bSuccess);
	vector<Phrase*> phraseList;
	Statistics m_stats;
	std::string m_filePath;
	std::vector<Phrase*> m_batch;
	std::vector<Phrase*>::iterator m_batchItr;
	void reverse();
	void equalise();
private:
	std::vector<Milestone*> m_mileStones;
	bool m_bReversed;
	
};

#endif LL_BACKEND



