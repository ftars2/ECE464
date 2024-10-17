#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <map>
#include <cstdbool>

using namespace std;

// Sort method for non Alphabetic gate names
void Numsort(vector<string> &In) {
  string word, wordNum;
  string word2, wordNum2;
  int k = In.size();
  int i = 0;
  vector<string> temp = In;

  while (i != k) {
    word = temp[i];
    wordNum = word.substr(0,2);
    int temp1,temp2;
    temp1 = stoi(wordNum);
    for (int j = 0; j < k; j++) {
      word2 = temp[j];
      wordNum2 = word2.substr(0,2);
      temp2 = stoi(wordNum2);
      if (temp2 > temp1) {
        temp[j] = word2;
        temp1 = temp2;
      }
    }
    i++;
  }
  //cout << temp.size() << endl;
}

// Separates the gate data
void gateData(string &line, string &gateName, string &gate, string &input) {
  size_t pos = line.find('=');
  gateName = line.substr(0,pos-1);
  size_t pos2 = line.find('(');
  size_t posend = line.find(')');
  int dif = pos2 - pos - 2;
  gate = line.substr(pos + 1);
  gate.erase(gate.begin());
  int end = posend - pos2 - 1;
  input = line.substr(pos2+1, end);
  //gatelist.push_back(line);
  gate = gate.substr(0, dif);
}

// Deciphers the total amount of inputs for the gate
void numInputs(int &inputSize, string gateInput) {
  size_t pos3 = gateInput.find(',');
  if (pos3 != string::npos) {
    pos3 = gateInput.find(',', pos3+1);
    if (pos3 == string::npos) {
      inputSize = 2;
    } else {
      inputSize = 3;
    }
  } else {
    inputSize = 1;
  }
}

// Checks the type of gate and prints it
void gatesList(list<string> &gates, vector<string> &gatelist) {
  int i = gates.size() - 1;
  int n = 0;
  bool junk = true;
  while (i != -1) {
    string line = gates.front();
    while (junk == true) {
      if (line == "" || line[0] == '#') {
        gates.pop_front();
        line = gates.front();
        i--;
      } else {
        junk = false;
      }
    }

    string gateName,gateType,gateInput;
    gateData(line, gateName, gateType, gateInput);
    gatelist.push_back(line);
    numInputs(n,gateInput);

    // Checks for total amount of inputs into each gate
    cout << n << " Input " << gateType << " gate with input " << gateInput << endl;
    gates.pop_front();
    i--;
  }
}

// Puts each gate into a level based on the inputs
void levelizer(string line, map<int,vector<string>> &levels, int maxLevel, int currentLevel, vector<string> &remaiderData) {
  vector<string> level0, nextLevel, totGateInputs;
  string gateName, gate, input, gateInputs, restInputs;
  int size = 0;
  size_t pos;
  numInputs(size,line);
  gateData(line, gateName, gate, input);
  level0 = levels[0];

  // Separates the inputs into a vector for easier parsing
  for (int i = 0; i < size; i++) {
    pos = input.find(',');
    gateInputs = input.substr(0,pos);
    if (pos != string::npos) {
      restInputs = input.substr(pos);
      restInputs.erase(0,2);
      input = restInputs;
    }
    totGateInputs.push_back(gateInputs);
  }
  
  // checks for current level
  int comp = 0;
  for (int i = 0; i < totGateInputs.size(); i++) {
    for (int j = 0; j < level0.size(); j++) {
      if (totGateInputs[i] == level0[j]) {
        comp++;
        break;
      }
    }
  }

  if (maxLevel > 1) {
    for (int k = 1; k < maxLevel; k++) {
      level0 = levels[k];
      for (int i = 0; i < totGateInputs.size(); i++) {
        for (int j = 0; j < level0.size(); j++) {
          if (totGateInputs[i] == level0[j]) {
            comp++;
          }
        }
      }
    }
  }

  if (comp == size) {
    levels[maxLevel].push_back(gateName);
  } else {
    remaiderData.push_back(line);
  }
}

// Prints the map containing the levels
void printMap(map<int, vector<string>> temp) {
  for (auto a = temp.begin(); a != temp.end(); a++) {
    cout << endl << "Level " << (*a).first << ": ";
    vector<string> inputLevel = (*a).second;
    for (int j = 0; j < inputLevel.size(); j++) {
      cout << inputLevel[j] << " ";
    }
    cout << endl;
  }

}

// Gets the value of a specific gate
void gateOutcome (string gateName, int gateSize, map<string,int> &temp) {
  int code;
  vector<int> inputs;
  if (gateName == "AND") {
    code = 1;
  } else if (gateName == "OR") {
    code = 2;
  } else if (gateName == "NOT") {
    code = 3;
  } else if (gateName == "XOR") {
    code = 4;
  }

  switch (code) {
    case 1:
      cout << "AND" << endl;
      for (int i = 0; i < inputs.size(); i++) {
        if (inputs[i] != 1) {
          temp[gateName] = 0;
          break;
        }
      }
      temp[gateName] = 1;
      break;
    
    case 2:
      cout << "OR" << endl;
      for (int i = 0; i < inputs.size(); i++) {
        if (inputs[i] == 1) {
          temp[gateName] = 1;
          break;
        }
      }
      temp[gateName] = 0;
      break;

    case 3:
      cout << "NOT" << endl;
      for (int i = 0; i < inputs.size(); i++) {
        if (inputs[i] == 1) {
          temp[gateName] = 0;
        } else {
          temp[gateName] = 1;
        }
      }
      break;

    case 4:
      int totalOnes = 0;
      cout << "XOR" << endl;
      for (int i = 0; i < inputs.size(); i++) {
        if (inputs[i] == 1) {
          totalOnes++;
        }
      }
      if (totalOnes%2 == 0) {
        temp[gateName] = 0;
      } else {
        temp[gateName] = 1;
      }
      break;
  }

}

// Puts the gate inputs, type and name into one data structure
void gateMapValues(map<string,vector<string>> gateMapData, vector<string> gatesList) {
  string line, gateName, gate, input;
  line = gatesList.front();
  gateData(line, gateName, gate, input);

}
void outputfaults(map<string, int> temp){
  cout<<endl<<"all potential faults in the circuit:"<<endl;

  for (auto a = temp.begin(); a != temp.end(); a++) {
    cout<< endl<< "node "<<(*a).first<<" stuck at 0/1.";
  }

return;
}
int main() {
  // Only used for the file reading
  ifstream Myfile;
  Myfile.open("c17.bench");
  vector<string> inputs;
  vector<string> outputs;
  
  // Only used in file reading and will be empty at the end
  list<string> gates;//extra

  // Will contain all relevant data for future uses
  vector<string> gatelist;//original list
  map<string,int> gatevalue;//a=x
  map<int,vector<string>> levelization;// ordered by levilzation

  // Copies the data from the file into a vectors
  if (Myfile.is_open()) {
    string word;
    while (getline(Myfile, word)) {
      if (word[0] == 'I') {
        size_t pos = word.find('(');
        word = word.substr(pos+1);
        pos = word.find(")");
        word.erase(pos);
        inputs.push_back(word);
      } else if (word[0] == 'O') {
        size_t pos = word.find('(');
        word = word.substr(pos+1);
        pos = word.find(")");
        word.erase(pos);
        outputs.push_back(word);
      } else {
        gates.push_back(word);
      }
    }
    Myfile.close();
  }

  // Prints the vectors
  cout << "Inputs: ";
  for (int i = 0; i < inputs.size(); i++) {
    cout << inputs[i] << " ";
    levelization[0].push_back(inputs[i]);
  }

  cout << endl << "Outputs: ";
  for (int i = 0; i < outputs.size(); i++) {
    cout << outputs[i] << " ";
  }
  cout << endl << "Gates: " << endl;
  
  gatesList(gates,gatelist);

  string temp = gatelist[0];
  if (isdigit(temp[0])) {
    Numsort(gatelist);
  } else {
    sort(gatelist.begin(), gatelist.end());
  }

  // Testing
  int currLevel = 0;
  int totLev = 1;
  int k = gatelist.size();
  vector<string> temp4 = gatelist;
  vector<string> temp5;

  string tempxy = "NOT";
  int sizet = 8;

  //gateOutcome(tempxy, sizet);

  while (k != -1) {
    levelizer(temp4[0],levelization,totLev,currLevel, temp5);
    temp4.erase(temp4.begin());
    if (temp4.size() == 0) {
      temp4 = temp5;
      k = temp5.size();
      totLev++;
      currLevel++;
      temp5.erase(temp5.begin(),temp5.end());
    }
    k--;
  }

  printMap(levelization);
  outputfaults(gatevalue);
  return 0;
}
