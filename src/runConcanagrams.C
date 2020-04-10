//Author: Chris McGinn (2020.04.05)
//Contact at chmc7718@colorado.edu or cffionn on skype for bugs

//c+cpp
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <vector>

//Local
#include "include/checkMakeDir.h"
#include "include/stringUtil.h"

const int doGlobalDebug = false;

std::map<std::string, int> scrabbleFileToMap(std::string fileName)
{
  std::map<std::string, int> scrabbleMap;
  std::ifstream file(fileName.c_str());
  std::string tempStr;
  while(std::getline(file, tempStr)){
    while(tempStr.find(" ") != std::string::npos){tempStr.replace(tempStr.find(" "), 1, "");}
    if(tempStr.find(",") == std::string::npos) continue;
    std::string valStr1 = tempStr.substr(0, tempStr.find(","));
    std::string valStr2 = tempStr.substr(tempStr.find(",")+1, tempStr.size());

    if(scrabbleMap.count(valStr1) != 0){
      std::cout << "Please check input \'" << fileName << "\', letter \'" << valStr1 << "\' " << std::endl;
      return scrabbleMap;
    }

    scrabbleMap[valStr1] = std::stoi(valStr2);
  }  
  file.close();
  
  return scrabbleMap;
}

std::string convertInputStr(std::string inStr)
{
  std::string lower = "abcdefghijklmnopqrstuvwxyz";
  std::string upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

  for(unsigned int i = 0; i < inStr.size(); ++i){
    if(lower.find(inStr.substr(i,1)) != std::string::npos){
      inStr.replace(i, 1, upper.substr(lower.find(inStr.substr(i,1)),1 ));
    }
  }
  
  return inStr;
}

int runConcanagrams(const int nPlayers, const bool doJudgePrompts)
{
  if(doGlobalDebug) std::cout << "FILE, LINE: " << __FILE__ << ", " << __LINE__ << std::endl;
  
  std::cout << "Preparing for Concanagrams, " << nPlayers << " players..." << std::endl;
  
  std::srand(unsigned(std::time(0)));

  const std::string dateStr = getDateStr();
  checkMakeDir check;
  check.doCheckMakeDir("output");
  check.doCheckMakeDir("output/" + dateStr);

  const std::string timeStr = getTimeStr();
  std::string outGameLogName = "output/" + dateStr + "/gameLog_" + dateStr + "_" + timeStr + ".log";
  std::string outPromptLogName = "output/" + dateStr + "/promptLog_" + dateStr + "_" + timeStr + ".log";

  std::ofstream gameLog(outGameLogName.c_str());
  std::ofstream promptLog(outPromptLogName.c_str());
  gameLog << "Preparing for Concanagrams, " << nPlayers << " players..." << std::endl;
    
  
  //  const std::string scrabbleFreqFileName = "input/scrabbleFreq.txt";
  const std::string scrabbleFreqFileName = "input/bananagramsFreq.txt";
  std::map<std::string, int> scrabbleFreqMap = scrabbleFileToMap(scrabbleFreqFileName);

  const std::string scrabblePointFileName = "input/scrabblePoint.txt";
  std::map<std::string, int> scrabblePointMap = scrabbleFileToMap(scrabblePointFileName);

  const std::string promptFileName = "input/prompts.txt";
  std::ifstream promptFile(promptFileName.c_str());
  std::vector<std::string> promptVect;
  std::string tempStr;
  while(std::getline(promptFile, tempStr)){
    if(tempStr.size() > 0){
      promptVect.push_back(tempStr);
    }
  }
  promptFile.close();

  std::random_shuffle(promptVect.begin(), promptVect.end());
  
  const int nTilesPerPlayer = 21;
  int nTiles = 100;

  double answerTime = 60.;
  int updates = 6;
  int intervals = answerTime/updates;
  
  double tilesPerNPlayersFrac = 7*4./100.;//From scrabble

  while(nTilesPerPlayer*nPlayers/(double)nTiles > tilesPerNPlayersFrac){
    nTiles += 100;
  }

  if(doGlobalDebug) std::cout << "FILE, LINE: " << __FILE__ << ", " << __LINE__ << std::endl;

  std::cout << nTiles << " tiles for " << nPlayers << " players..." << std::endl;
  gameLog << nTiles << " tiles for " << nPlayers << " players..." << std::endl;
  int mult = nTiles/100;
  std::cout << " (multx" << mult << ")" << std::endl;
  gameLog << " (multx" << mult << ")" << std::endl;
  
  int nRound = 1;

  std::vector<int> playerOrder, playerScores;
  for(int pI = 0; pI < nPlayers; ++pI){
    playerOrder.push_back(pI);
    playerScores.push_back(0);
  }
  
  while((nRound <= (int)promptVect.size() || doJudgePrompts) && nRound <= nPlayers){
    std::cout << std::endl;
    gameLog << std::endl;
    std::cout << "Round " << nRound << "!" << std::endl;
    gameLog << "Round " << nRound << "!" << std::endl;
    std::cout << "Scores: " << std::endl;
    gameLog << "Scores: " << std::endl;
    for(int pI = 0; pI < nPlayers; ++pI){
      std::cout << " Player " << pI << ": " << playerScores[pI] << std::endl;
      gameLog << " Player " << pI << ": " << playerScores[pI] << std::endl;
    }


    int judgePos = (nRound-1)%nPlayers;
    std::cout << "Player " << judgePos << " is judge" << std::endl;
    gameLog << "Player " << judgePos << " is judge" << std::endl;
    
    if(!doJudgePrompts){
      std::cout << "Prompt: \'" << promptVect[nRound - 1] << "\'" << std::endl;
      gameLog << "Prompt: \'" << promptVect[nRound - 1] << "\'" << std::endl;
    }
    if(doJudgePrompts){
      std::string promptStr = "";
      while(promptStr.size() == 0){
	std::cout << "Judge (Player " << judgePos << "), enter prompt: ";
	gameLog << "Judge (Player " << judgePos << "), enter prompt: ";
	std::getline(std::cin, promptStr);
	if(promptStr.size() == 0){
	  std::cout << " Please enter a valid prompt" << std::endl;
	  gameLog << " Please enter a valid prompt" << std::endl;
	  promptStr = "";
	}
	else{
	  std::string yesNoStr = "";
	  while(yesNoStr.size() == 0){
	    std::cout << "Is \'" << promptStr << "\' the prompt you wanted (y/n)?" << std::endl;
	    gameLog << "Is \'" << promptStr << "\' the prompt you wanted (y/n)?" << std::endl;
	    std::getline(std::cin, yesNoStr);
	    if(yesNoStr.size() == 1){
	      if(yesNoStr.find("n") != std::string::npos){
		promptStr = "";
	      }
	      else if(yesNoStr.find("y") == std::string::npos){
		std::cout << "Please enter \'y\' or \'n\'" << std::endl;
		gameLog << "Please enter \'y\' or \'n\'" << std::endl;
		yesNoStr = "";
	      }
	    }
	    else{
	      std::cout << "Please enter \'y\' or \'n\'" << std::endl;
	      gameLog << "Please enter \'y\' or \'n\'" << std::endl;
	      yesNoStr = "";
	    }
	  }	  
	}	
      }      
      std::cout << "Prompt: \'" << promptStr << "\'" << std::endl;
      gameLog << "Prompt: \'" << promptStr << "\'" << std::endl;
      promptLog << promptStr << std::endl;
    }
    
    std::vector<std::string> letterVect;
    for(auto const & val : scrabbleFreqMap){
      for(int vI = 0; vI < val.second*mult; ++vI){
	letterVect.push_back(val.first);
      }
    }

  if(doGlobalDebug) std::cout << "FILE, LINE: " << __FILE__ << ", " << __LINE__ << std::endl;
    std::random_shuffle(letterVect.begin(), letterVect.end());
    
    std::vector<std::string> letterStrs;
    for(int pI = 0; pI < nPlayers; ++pI){
      letterStrs.push_back("");
    }
    
    for(int lI = 0; lI < nTilesPerPlayer; ++lI){
      for(int pI = 0; pI < nPlayers; ++pI){
	if(playerOrder[pI] == judgePos) continue;

	int pos = lI*nPlayers + pI;
	letterStrs[playerOrder[pI]] = letterStrs[playerOrder[pI]] + letterVect[pos] + ",";	
      }
    }

    letterStrs[judgePos] = "JUDGE";
    
    for(int pI = 0; pI < nPlayers; ++pI){
      std::cout << " Player " << pI << ": \'" << letterStrs[pI] << "\'" << std::endl;
      gameLog << " Player " << pI << ": \'" << letterStrs[pI] << "\'" << std::endl;
    }

    std::cout << "Prepare answers now!" << std::endl;
    for(int tI = 0; tI < updates; ++tI){
      std::cout << " " << answerTime - tI*intervals << " seconds!!!" << std::endl;
      gameLog << " " << answerTime - tI*intervals << " seconds!!!" << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(intervals*1000));
    }
    
    std::string answerString = "";
    while(answerString.size() == 0){
      std::cout << "Answer?" << std::endl;
      gameLog << "Answer?" << std::endl;
      std::getline(std::cin, answerString);      
      answerString = convertInputStr(answerString);
      if(answerString.size() == 0) continue;
      
      if(doGlobalDebug) std::cout << "FILE, LINE: " << __FILE__ << ", " << __LINE__ << std::endl;

      std::vector<int> possiblePlayers, possibleScores;
      for(int pI = 0; pI < nPlayers; ++pI){
	std::string tempAnswerString = answerString;
	std::string tempPlayerStr = letterStrs[pI];
	std::vector<std::string> letters;
	while(tempPlayerStr.find(",") != std::string::npos){
	  letters.push_back(tempPlayerStr.substr(0, tempPlayerStr.find(",")));
	  tempPlayerStr.replace(0, tempPlayerStr.find(",")+1, "");
	}

	if(doGlobalDebug) std::cout << "FILE, LINE: " << __FILE__ << ", " << __LINE__ << std::endl;


	bool allFound = true;
	int tempScore = 0;
	if(doGlobalDebug) std::cout << "FILE, LINE: " << __FILE__ << ", " << __LINE__ << std::endl;

	for(unsigned int lI = 0; lI < tempAnswerString.size(); ++lI){
	  bool letterFound = false;
	  for(unsigned int lI2 = 0; lI2 < letters.size(); ++lI2){
	    if(doGlobalDebug) std::cout << "FILE, LINE: " << __FILE__ << ", " << __LINE__ << ", " << lI << "/" << tempAnswerString.size() << ", " << lI2 << "/" << letters.size() << std::endl;

	    if(tempAnswerString.substr(lI, 1).find(letters[lI2]) != std::string::npos){
	      letterFound = true;
	      letters.erase(letters.begin()+lI2);
	      break;
	    }
	  }
	
	  if(doGlobalDebug) std::cout << "FILE, LINE: " << __FILE__ << ", " << __LINE__ << std::endl;

	  if(letterFound) tempScore += scrabblePointMap[tempAnswerString.substr(lI,1)];
	  else{
	    bool hasBlank = false;
	    for(unsigned int lI2 = 0; lI2 < letters.size(); ++lI2){
	      if(letters[lI2].find("BLANK") != std::string::npos){
		hasBlank = true;
		letters.erase(letters.begin()+lI2);
		break;
	      }
	    }

	    if(doGlobalDebug) std::cout << "FILE, LINE: " << __FILE__ << ", " << __LINE__ << std::endl;

	    if(!hasBlank){
	      allFound = false;
	      break;
	    }
	  }
	}

	if(doGlobalDebug) std::cout << "FILE, LINE: " << __FILE__ << ", " << __LINE__ << std::endl;

	if(allFound){
	  possiblePlayers.push_back(pI);
	  possibleScores.push_back(tempScore);
	}
      }
    
      if(doGlobalDebug) std::cout << "FILE, LINE: " << __FILE__ << ", " << __LINE__ << std::endl;

      if(possiblePlayers.size() == 0){
	std::cout << "Given answer \'" << answerString << "\' is not compatible w/ any players letter set, please re-input." << std::endl;
	gameLog << "Given answer \'" << answerString << "\' is not compatible w/ any players letter set, please re-input." << std::endl;
	answerString = "";
      }
      else if(possiblePlayers.size() != 1){	
	int playerWhoWon = -1;
	while(playerWhoWon < 0){
	  std::cout << "Which player corresponds to that answer: " << std::endl;
	  gameLog << "Which player corresponds to that answer: " << std::endl;
	  for(unsigned int pI = 0; pI < possiblePlayers.size(); ++pI){
	    std::cout << " " << pI << ": Player " << possiblePlayers[pI] << std::endl;
	    gameLog << " " << pI << ": Player " << possiblePlayers[pI] << std::endl;
	  }
	  std::string playerWhoWonStr;
	  std::getline(std::cin, playerWhoWonStr);
	  playerWhoWon = std::stoi(playerWhoWonStr);
	  if(playerWhoWon < 0 || playerWhoWon >= (int)possiblePlayers.size()){
	    std::cout << "Invalid input, please input again." << std::endl;
	    gameLog << "Invalid input, please input again." << std::endl;
	    playerWhoWon = -1;
	  }
	  else{	    
	    possiblePlayers.push_back(possiblePlayers[playerWhoWon]);
	    possibleScores.push_back(possibleScores[playerWhoWon]);
	    int nPlayerSub = possiblePlayers.size()-1;
	    for(int pI = 0; pI < nPlayerSub; ++pI){
	      possiblePlayers.erase(possiblePlayers.begin());
	      possibleScores.erase(possibleScores.begin());
	    }	    
	  }
	}       
      }
      
      if(possiblePlayers.size() == 1){
	std::cout << "Player " << possiblePlayers[0] << " won!" << std::endl;
	gameLog << "Player " << possiblePlayers[0] << " won!" << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	std::cout << "Player " << possiblePlayers[0] << " earned " << possibleScores[0] << " points!" << std::endl;
	gameLog << "Player " << possiblePlayers[0] << " earned " << possibleScores[0] << " points!" << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	playerScores[possiblePlayers[0]] += possibleScores[0];

      }
    }    
    
    playerOrder.push_back(playerOrder[0]);
    playerOrder.erase(playerOrder.begin());

    ++nRound;
    promptVect.erase(promptVect.begin());
  }
  if(doGlobalDebug) std::cout << "FILE, LINE: " << __FILE__ << ", " << __LINE__ << std::endl;

  if(!doJudgePrompts){
    std::cout << "Out of prompts (" << promptVect.size() << " defined prompts)" << std::endl;
    gameLog << "Out of prompts (" << promptVect.size() << " defined prompts)" << std::endl;
  }

  std::cout << "FINAL SCORES: " << std::endl;
  int maxScore = 0;
  int winner = -1;
  for(int pI = 0; pI < nPlayers; ++pI){
    std::cout << " Player " << pI << ": " << playerScores[pI] << std::endl;
    gameLog << " Player " << pI << ": " << playerScores[pI] << std::endl;
    if(playerScores[pI] > maxScore){
      maxScore = playerScores[pI];
      winner = pI;
    }
  }
  std::cout << "PLAYER " << winner << " WINS!!!!!!!" << std::endl;  
  std::cout << "RUNCONCANAGRAMS COMPLETE. return 0." << std::endl;

  gameLog.close();
  promptLog.close();

  return 0;
}

int main(int argc, char* argv[])
{
  if(argc != 3){
    std::cout << "Usage: ./bin/runConcanagrams.exe <nPlayers> <doJudgePrompts>" << std::endl;
    std::cout << "TO DEBUG:" << std::endl;
    std::cout << " export DOGLOBALDEBUGROOT=1 #from command line" << std::endl;
    std::cout << "TO TURN OFF DEBUG:" << std::endl;
    std::cout << " export DOGLOBALDEBUGROOT=0 #from command line" << std::endl;
    std::cout << "return 1." << std::endl;
    return 1;
  }
 
  int retVal = 0;
  retVal += runConcanagrams(std::stoi(argv[1]), std::stoi(argv[2]));
  return retVal;
}
