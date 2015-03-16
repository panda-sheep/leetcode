/**********************************************************************

 All DNA is composed of a series of nucleotides abbreviated as A, C, G, 
 and T, for example: "ACGAATTCCG". When studying DNA, it is sometimes useful 
 to identify repeated sequences within the DNA.

 Write a function to find all the 10-letter-long sequences (substrings) that 
 occur more than once in a DNA molecule.

 For example,
 
 Given s = "AAAAACCCCCAAAAACCCCCCAAAAAGGGTTT",
 
 Return:
 ["AAAAACCCCC", "CCCCCAAAAA"].

**********************************************************************/

#include <iostream>
#include <vector>
#include <string>
#include <cstring>

using namespace std;

const int SUBSTRLEN = 10;
const int MAXHASHSIZE = 100000;

typedef struct node {
	int startIdx;
	int count;
	struct node *next;
} Node;

/* you need to choose a good hash function, 
 * so string s can be nearly random distributed in hash.
 * A bad hash function will cause time complexity O(n^2).
 */
static int getHash(string s, int hashSize) {
	int len = s.size();
	int ans = len;

	for (int i = len-1; i >= 0; i--) {
		ans = ans ^ ((ans<<5)+(ans>>2)+s[i]) % hashSize;
	}

	return ans > 0 ? ans : 1;
}

vector<int> getIdxAndFreeNode(Node **hash, int hashSize) {
	vector<int> idxs;

	for (int i = 0; i < hashSize; i++) {
		Node *cur = hash[i];
		while (cur) {
			if (cur->count > 1)
				idxs.push_back(cur->startIdx);
			Node *tmp = cur;
			cur = cur->next;
			delete tmp;
		}
	}

	delete []hash;
	return idxs;
}

vector<string> getSubStr(string s, vector<int> idxs) {
	vector<string> subStr;
	vector<int>::const_iterator it;

	for (it = idxs.begin(); it != idxs.end(); it++) {
		string sub = string(&s[*it], SUBSTRLEN);
		subStr.push_back(sub);
	}
	
	return subStr;
}

vector<string> findRepeatedDnaSequences(string s) {
	int hashSize;
	Node **hash;
	vector<int> idxs;

	hashSize = s.size()*2 > MAXHASHSIZE ? MAXHASHSIZE : s.size()*2;
	hash = new Node *[hashSize];
	memset(hash, 0, hashSize*sizeof(Node *));

	for (int i = 0; i < (int)s.size()+1-SUBSTRLEN; i++) {
		string substr = string(&s[i], SUBSTRLEN);
		int index = getHash(substr, hashSize)%hashSize;
		int found = 0;
		Node *cur = hash[index];
		while (cur) {
			string substr2 = string(&s[cur->startIdx], SUBSTRLEN);
			if (substr == substr2) {
				++(cur->count);
				found = 1;
				break;
			}
			cur = cur->next;
		}
		if (!found) {
			Node *newNode = new Node;
			newNode->startIdx = i;
			newNode->count = 1;
			newNode->next = hash[index];
			hash[index] = newNode;
		}
	}

	idxs = getIdxAndFreeNode(hash, hashSize);
	return getSubStr(s, idxs);
}

int main(int argc, char *argv[])
{
	string s1 = "AAAAACCCCCAAAAACCCCCCAAAAAGGGTTT";
	string s2 = "GATGGATACTGCATTCGAACCAGAGCCGGCTTTTGCGGGACTAGCATGAGGGACTTGGCTGTTGAGGCTGTACGAGGTCAGTCCTCCGGCAGTGCTATCGCAGGAATTTTTGCAACTCCACTGCTTATAATCCACCAAGTCCAGACTCAAAGCTCAACTCGGGGATCGCACGGTATGGTCACTGTCGCGCATGCAGTAATAGTCCAGACGAACGCACATTGGTCGTCCCCTGAGCCCGTGCCAGCCTAATACTTCTTATGCCTGCGTAAGTAGACTTTGCCAACGTAATCTCATCCTTATGCCAGATTATTAGTTCATTGAATGTCGGTCGCCGGCGCTCCCGCATTTCTTATCCGCGTATCTTGGGGTCAAGACGTCCCCAGCTTGTTAATACAAGCTACTTTCCCTCGCAATTACTAAGTTCGTGACAAGCGAATCACGCTAAGATGTTATTGGACTCTACAGAAATATTGAATTGACAACATTCGTCTGTTCAGATCGTCGTTCACGCCACTGATAGCGCAGCTCGAGCACTCTGGAGCCACAATGCGGAATGTCCAGAACCTTTGCGCAAGAGTCCGTGGAAAGCATAATCGTGAACAGAATGGCTAGCCGAGGTGCGCAAGGATAGGACCGTCTACACAAAGTATGGGCACCACGCACATCGACACCCCGTAGTGTGTCAGTCGGCTCAGCGGCTAATGGGTTCGGCGTGAGGAATAGAAATAATAGGCAGTGGTGCCAATTGTGGGGTCTTCTTTTGACTTTCTCATCTCTCTATGAATCAGATCGGCCTCTCGCCCCCGCCGGCCCTCTGGCTTTTTTAAATCCTAGATTGTGCACGTGCCCCGGTTTCCTTCAAGGCAAGTGAAGCGCGTCTTTGCTCTAAACCCACGGCCGTTGCACGGCGCCGAACAGGTGTCTCGGTGCGACCGGAGTGAGCAAGTTCTGTCCGCATCGTATGATTATACCCCCTCCTGTCACGGCTCGGGACTTATCGCACCACAGATCAGCTCGCAGCCCCGCGCGAGTACTAGGGACGGGAGGGAAACCAAAGATAATCGTCTTTGCATGGGCCGGCATGTGAATCATTCGTATCATCTTCTGGAGTCTTGTCACGACGATTTTCGATACAGACTGTTGACCCATCTAATCGTGTTGTCAGTCTGGGAACCGTACTTTTTAACCCGTCGTTCGAGCGGCCCGATCAGGGATGCCCGCAGTGTACGGGCACATCGTCGTCTTGGGAACAAAAGCTTGACGGACACCTCTATGCAGACATGAGACGTGAGGCCCCTGCAATAGTGCGGTCACAGGGACCGGCTGTCGATCAGTAGGTATAATCTTGATGTTTGCTGGGAGATTAACAGAGGGCGGAGTTCCGCATCGCCTAACCACTGACAGTCATTGATAGACGCCTAAGTTTGTCCCTGTAGCTACAGTGGTGGCAAAGTGGCCTTGGACGGTTCTGCGCTTGTCAATAAGTCTGTCCCAATCACGAGTGAAAAACTAGCTAGGGTCGGTGATGTGTTTTCAATCATATTTCTCCATCCATCCGGGGCTCCCTAGTACGGAGGAAATCTCCGGGTAACTCTGGATCTCCAGCATTGCGTAAGCAAACCGCCAATCGGGCCGCAGTGAGTTCTTAAACTACGGTTTGGCCCTAATCGCACTATTGGGTGTTGTAGATACGATAGCAAGGTGATTATGAAATCAAGGCACGCACGACCTGTACGTTGATCGTCGAGTGCTCTCGAGTTACTTCATGGGTCAGCCATGCGATTGTCCAAATGGACCGGAAAGTACACTACAAATTGTACCACTGTGCGTTGTACCTCACAAGAACTGTTTGGGTCTACTTACTTTTTACTTGGATCTTTCCTGGTCTCCCTCAGCGTAATTATTCGACACAATGCTGCAGCTGCGTTGTAGTTTTGGCGGTACAGGAAAAATTCTTGTGAGCAACCAGGCCATTCCCTGGAAGGCAGTCCTTGCGAGTATGTTGAGATATATGCTGGGGATGAATTAGAACATTATGCCATCTAAAGTTTGGATTACCGGGGATTCGGCATACCAAATGGATTCGTTGATTATAGCCCCCCCACCTCCTTTTAGGTAAAATGCCCAATCCTGGCGCTGAGCAGGAGGCATGTTGGCCTCTTGTCCGGTAGTACGCTTGACTAGTTCCTAGCGGCGCAAATCACTTGGTCTCTGTCCGTCCTGAATGTTACAAAGCCATATACATGTGTGGAGGTCAAGACATTCCTTATCCGCCCCCCTCGCGATGCAGTTAGATTCGCATTCAAGGTTGAGACCGGAGACCTTCTAACCGGATTTTGGAGTATAGCCCCTTGATAAGAGAAGGAACCATGCTGGGTCTCACGACTATTGAGTTCGGGAAAAGGTGAATGCTCAACGCTAACGCAGTTTGTTACGCCTGGCGGAATAACGTCAGGGACAAACTATATTCTGGCGCCCCAGTGTGGGCTCTTTGACGACATAGGACGGATTAGGCCGGTCTCAACCGCCTCGTAACCCAGGAAGCGGTTCTACTCCCGGCTACTTTTTTGGAGTGTGCAAGGACGTTGCACACAGTGGGTGTCAGATCTAGCCCGTCACATGCAAGTGGTCATATGGATCCCATAATACTCACTGAGTGTCTCGCCAACGGGACTATTAATAGACACGGTGATAGACGGTAGGAATTGTGAGATTCATAATTAGTAACAGTAGGAGCGCCGTAGGCCACGGACCGATACATCGGAACCCTTCGCCGAATACGTTAAGGGTTTGCAACCGGGGGTGCATCTAATCCTTGGGGCTGATCTGTCAAAGGCGTCTCATGCGTGATTATATTAGCGAGAACCTGCATCAATGCTTTAAATAGACAAACCGGTTAATTTGCCAAGAGTTGGGACTACCTGACGGCAAGAGATGGTTAATTGGGCGATTACTTCACGGGTTTGTCTCGATTAAAGAACTAGGATTGCTGTTGTCTTACAAGTTCAGTCATTATCCTTCTGCTATAGCTCTGGATTAGCAATTGGTTTGGGGAAAACCTTTCTCCGTACGAGTAAGGCTGTCGGTAGCCATACTGACTGATCCCGAACACAAAACACTATTCTGTGGAACCAGCAGGTATTAGCATACTGCCCAGTATTCCCGAGTTACGGTTGACTCGGGTCTTAAGCATTTTAAACTGTCCCGATAACCCATTGACTAAGTCCATACATCAGACAAGCTCATGCATCCACTTATACCCGGTGAGAGTAGTTTATGACCAGTAACCGTTACCGAGCCACAGCCACCGCGTGGTGTTGCGGCGCTGTACTATATTATTACTGATGCGGTCCCTAGAGGACTACGCTCCAAGCAAACTTATCTTATTGAAAGTATACACCTAGCAACCTGGGCCGGATTCAATCCGGGATCTGCTCCCCTAGAGCGTTTAATTCGGACCCCTAGTACATAACATTCGGAAATTGGTTCTGGCTTTATGTGGCAATCTGTAATGCAGAAAACATGCTATCGCGCATAAAACCAGTTAAGCTTGATCTCAGATAGTAAAATTCCTGTACCCTCTACAATCCCCCAGAGAAGCTCAGTATACGAACGGAGGAGTAGACTTCGTTACTTGATTCTGGCTACCTCAAAGTCCTCCCTCGAGTTAGAACATGGTGCTCCGTCAGAGGGTTCATTATGGCCGACACGCGAGCTTGCGCTCTGTATGTATGCCGGGGTCCTCTGTCCTGGTGGAGACTGACCATGTACCCCTTATGCCCGGAGAATACCACCTGCTCCACGTGATGCAGCACTGATTGCTCATGGGCGCTCAGCATAGAGATATCTGAGTAGTCCGATACACTTACAGACCGTAAATGCCACTCGGGTTCCTAGCAGAGTGAGGGGTTGCGAATTTTCTGCACATGTAAATTGGGCGTAGACACAACTGTAAAATGCGTCGTAAAAATACAGCTAAGCACCGAACAACGGGATAGGTATTTAGCTAACGTAATTAGTTGTAGAGACACGTAAGCATCCTAGGTCTGACGCTCCCACTATTACGGTGAGTTTGACGCAGAAGTCTTGGTTATGATGCCCTGGATCTGTGAGCTCGGCCTTCCCTGTACAGTGGAAAGATCTCGTTGCTCCGATTGACGGGTTCTGTTTTGTTCGTCCGCGGGGAGTTTCAGATTAACGTCAAGTAGTGCTGGACAGGTCAATAAGATCGCCTCCCTCAACCGTAAGGTCTCGAAGCAAAAGCTCGGTTTGGCATGGAAAACGGAGATTGGCGCTAAGCGACACGGACTGGGGAAGGAAGGAGAGCACAATCTCAGCTTCAAAATGTCCCACCCAGGCTGGGCCCTTCCACGACCTGATTATAATATCTGTATGTGCAAGCGGAAGATTCCCGGTACGTAGAAGAACTCAGCACAGAGACCCCCCGGGACATCGACATCGATTGAACTGCGGGGATGTCAACATAGAACGCGGCGCTCGTAATAAGGGATACCCGGGATAATACACGTCAGGAGGTAACCTGAAGGCGAAGCTCTACAGAGAAAGCGAACCTACGCGTTGATTTAGACCAGCCTACATCACGCTGAGCGCTAATCCACCCGGTAGATAAGGTGAAACACAGATATGGCTGGATTAGATACGGGCGTCCCGTTGCTCATTGGAGTGCGTGTTCGTAACAGACCCCAATAAGGCCGCGTCTGACCAATCATGCGAGAACAAAGACATCCGAACGTCATAAACTATAGATACAAACCACAGGGGAACGTTGCATTGGGCTTGGTCGAAGAATCAATTGAGGTCTGATTGGGAGGCGTCCTGTATAGGTCAATTCTTCTATGGGTACTTCCCCCAGGGCTCCCCATCGACCACTATGCCTCGAGTCTGTCTTGATTTCTGCATGTGCAGACAAGCCTTGACCATACGGGCAATTACTGAGGTCGCCATTCGTGCTACTATATCGTCTACTATTTGAGTGCAAATTTGCCAGACCTGTGGCCCCTTGTGTCAGGGGAGCACATATTAACTAGACCTAGACCCATACGGGTCACACATCTCGGCCGGGAAAATTGGTTATCTGGGTCTATTCTGTACATACTTCGATTTCCATACAAGGTGGATAACTTTTGGCACAGAACGTAGCAGTCGATCTCTACATCCCACGACCTAGATAATCTCCTACCCGTGGTGCGCTATCGGTGGCATCGAGTTGTAATTTTCAGTAGTCCTGTGCACCCGGGGGTGTAGTGACAGCGATTCATCCCCAACGATGGGGGCCACGGATCCACAGCACGGCCACCCGCGGGCCCTAGGCCGACCTATCTACATTGGATCTACCCCGACGAGGCCATGATATAAATAATCCATAGCTAAGCGCATGATTGACTCTATGTCACTCCGTTCTCTAGTGTCTCCATTTCTACTACTTTCGTCGAGCAAGTGTAACAAGAATGCAGTACCTCTTCCAGTTCAGTGGAATCAGGAGCATTGTGCGGCTAAGTCATTAGACCTCATCGCTCCGCGCCGCCATTGTATTAATTAGACGAGGTACTACCCTGTGTCTTAACCGTCGCAGTGGCAGACACCAAGTAAAGTCATGGGGTAGTTCTATCACTAACCAACGGGATCAAGGAGTAGGGACCAGACCACCTATGACGTCCCATCCAACGCGAAGCGCCGGCTCTGACGGGGAGCCGGTAGAATAGGGCCGGCAATCGGGGTCTTGAGGGAGTGTCGAACATCGCGACATGCGAATTCAGTGTTTGGCAGGATTGGGTAGGCAGGCTTCCTGGGGCTGTGCTTTCTTGTTAGTGTACGGACCATACCTTGGGTGAGAGTCTGGGGCCATAGCCTGGATAAAGCATTCCCGGCGCCTCCTTATGCTGTGGCAAGCGCTCCTACACAATGCATGATAACAACCGAGGTCGTCGCACTCACGCTCATGAACAAGAATCTCTACTAGGATCGCTTGCCGGGGGTACTTCGCACCGACTAGGTTCTGTCTTTATCGCTGATAATTTTTACTCAGGGTGCTCCGGCTCTGCACTAGTAATCATCCATGAACCTTTGGCGCGAGAGGCGGTCTTATTACCACGTTGTCATTGAGTACATACTCCGCCGGTCACGTGGTAAGACCAACCACGGACTGTTAGATTTAATGGGGACATATAGGTCCACACAATCAGTCAGACTTAACAGAACGGGCTCTTTTCGCTTTTGGAGCTCCTCTGTATAGGACATCGCTATTGAGCAAAGCATCCGATCGTAGTCGAATACCTCAAGTGAAGGCTTTGCCAAGGATTTACTCCCTGGTTACGTGATAGAGGTTCACCGCGTAGGTGAGGACCTGTGTGTGAAGTTTGTGCGGATTTCGCTGAATGATAGATTCATGGACTATCCGTCCTTTGAGATAGCGATTAGTCATCCTGCGAGACGTTACCATGTGCTACTAAAGAGACTATGAACCCAGCCACGTCATTAATCCGCAGGTAGTCTTGGACTTTTTGCTCGCAACCAACGTTTTTCTAATCCAAAGTTCCGTGCAAGACACGTACCATGCATGCAGCCAGAGTGCTGTCTCCCACGTTCGATGAAGGTAATAAGTGGGTCAAAGAGGGGGGACAGATTACGGGAGCGGAACTTTGTATGACTTATGAAATTAAGACTAGTTACGGGGATGCTCTGGTTATTGAACTGTACAATTTCGATAATGGCCGCGGTCTCGAATTTGCGTTCGCGGGCTCTTGACAATCTCCGTACAAATATACTCGGATACGATTGAGACCAATTAGCAGTCTGCGTTGGGGTTGTAGTCGCCGGTAGACATATCCCGCTAAAGAACGCGGGCGTATGCAGTAGTCAGCAATGCCGGTTGTCTCAAAGTAGTGCGTCCGAATTCATAAGGTATCGCCAGTCTCCCCCACAGTCTGCTTGAGAAGACGTGTAAGGGTCGCGACGGGTAGCTCTACTGGACTGTAATCGGTCGATTAGCACGGGAGGCTATCATACCTACCTCTTGCCTTGAATCGACTTATGGCTAGGCAACGTCACCGGGACTTACTTATCTGGTTTGCAGTGTTCTTTATAGTCAGGGTCCGAGGGTTCCTCTGCGAAGGCCCATCCGCAGAGTTACATAAGTTCCCCTTCGTGCCACAGCTCAGGTAGTTTTTAGAACATAACTAAAAAGGAATCAAGGCGCGGTCCTTCATACGAAATCATTTATAGGCCGGGATGGTGGTAAGGCCGCTACTTCAAGGTGCCTAGGAGAATTAGTACGGTTGTGGCCAATCTCTTCCGAAACAACATAAGATGGCACAGTCACTTTACGTAAGATCACGCCAAGTTGCGGAATGCATTATGGGTCGTCATCCCTGATTGCCTGACACGGATCTTATCTGCGCGTTCGTGTACGCGGTCAGTGTATCGAGTCGGCACGTATTCAGGGAGTTCTTAGTTGCAAAAGGCAGAAACCCTTTCCTCTCGGTCAGATCTCAAAAAATCGTCAGTTGGCTTTCCAGTATCAAAATAAGCGGAGTAAACTAGGGCCCCTTCCAGGAGGGGATGCGATATTTCTCTAAGATACCCGAGGAGCGAGTCCCGGCTAGTTCGGACCCACCGTTATTCAATATAACCGCCGTCTTTTCCTGCGATCAGGGGAAAGAGACTCCAGGTTTGGAGCACACTTGACGATATATAAACTGGCCTTGGTCCTTAAGACGTCCTAGAACGAAGTTTCAACACGGAGGTTCTCTGGAGATAGCCTGATTACCGAGCGGTCGCCCCTCCGGATCCACCAAACGGCTACCGTTCGACTCGTGCTATACTATTGTACGCGAGACAGGCCACAGAACTCGAAGGGGTATAATGTTGTTGCACCTGGCCTTATTACACCACTGACGGATTAACCAGCGCGTATAGTTACGGCGAACCAGCCTGTGCTAATTGATTTCTCGGCTCCACTTTGGTCAGGTCACTAAATGAGGATACACGCTTTATCAGCAAGAATTTAAATACTCAGCGAACGCGGTGAAGATCGTATCCCGGCCGTAGGGGAGGCGTTAGTGCTGCTATAGGGACAATTCATTGCCGAATTTTTTGCGCGCCAATCTAACCATGTGACGCGTCTTTACGCATGCTGCGACGAAGAAAGCCCGTTTAGGAATCCTCACAGCACTTACCATAGGCGCGAATCCTTCCGGTGACGACGCAACATACTGACCTGTGGACGGTTTGTATTTGCGCCGCATGCTTGATTCTTGCGGCGTAAGGGAGCGGCTTCGCAGACTACTGTAACCGGTCATAACGTCACTAAAGAAAGCTTACCCGGGCCAGAAACGGTTCCTCTCAATAGCCCACGAGGAATATTCGACCCTGTCGTCCCCGCCTCCGGCGGTCTTTACGACCCTCATCTTACTCTAATCCTACGTCCCGAATGTCTGTAAGCGTTAAGGCAGAGATACGATGGTGCCTTGCTAGCTTTCTTTATGATAATCGGAATTATGCGCACTCACCTTACGCCGGCAAATCACGGTAAGCCTTCCCCGAGACTATACACCCCATGTGAGCGACAGCAGTGAGTCAATTCGCAAAGAAAAGACCCTCCGTTGCGGATTCGGCGTCACGGGCGAAAGTCGTACAAGTAGGAACTACCCTGCTCCAAGTTACCTGTGTGAAACCTTTAGGGTGCAGCTTCAGATAACTACGGTCACGTTTTTTTCCATCCATGCACTAGGTAGGTTCAATACTCCAAACTGACTCTTGCGGCCAGAGGTGTTCGGAAGTGTACTGAGATTCGCACCCCGCCGAGATTGTCTACGATCAGTTGTTAGGCGGAATGAATGGGACTGTAACGGACCCACGCGCGTGTATACTTTTCCCTTAGGTCCTAAGATGGTTAGCATGTCCTGATAGTGCATCACCGTGCCTTGTATTTAGAGATACACTCAAGTCTGAAGGATCTTTTTCTATAGGATCGCAATAGGACGTGTCTATAGTAAGTACCAGGGCTTCCGTGCGTTCCGATCCGGGACGGCACTCTAATGTCGGAGAATCTGTCGGGGATATGAGGAATTTTACCACTATCACCTTGATCATTGGCCACGGCATGGCTGTGTATGGTCTGGAATTTATACATGAATTTCACATTCGAAACTATGCCGCAGACTCCCTGCGATCCCCTGAACCTGTCGGGGCTCGCGAGATTAAGTGCTTAGTCGTGTACGTAGCATTGGGCCATTCCAGAGCGGGAGCTGAGGGCCGAAGCCAACCCAAAGTTTTAAAAGGCGTCCTTAGGACGAGAGGCTAGTCATTAACCACGCCTTTGCGCGCATTAGTCACACCCGCCCTCTAGTGTTTCAGTGTTCACCGCCGCTATCGTGCTTATTGGCAACGGCGACACCCCATATATGGGACCTGGAACAGAAAGACGCGACAAATTATCCACGAGGCGTTCTCTACAATTTGTTATATCAGTCTAGGTTGTGGCGCCGCATTCTAACTAAGTTGCCACACCGCTCACAGCAGGCTTCTGTGCTAGTGTACGCGTGGCTCCGTGGCCAAAGAGTGCCAAACTTGGTTTGCACGCCATTACGTTTTCAGCTAGTCATAAACCAGGCATACACTTCACAAACTACACTTGCCATTCATTCTCTGCCAGAGATTTGCTTCAAAAGCAGAAGGTAGAACAGTTTCTCTAGGGGTTAAGAACGGGGCCGTCTACTAACCAGCTCCAACGTTATGTCACGTCACAAACGTATAATGTACGAAGTAGCATCGACCAGGCCGGCTATCTCAAACAGACGTGCCATCTTCCTCGCCTTAGGGGGTTCAGCTTCGAATTAGATCCCTTGATCTCGTCCACCGCCTCGAGTGATGTATATAGCTTGTCCTGGAAGCGTGTCCGATTTAGGCATAACAATGATGATATCTTACACCCCCACTCGTCTAATCTCTCTCACGCCCAATAGTTTTTCGTCCGCCCTCTGATGCCAGCACGCGGACAGTGGTCGTTTGCTCGCGATTCGGGTTGTCACTGCAATCACATGCCGGACTTGCAATTGCGCATGACGACAGGTCGACCTCAAACTATAATACTAGCCTCCAGTCTGTCGAAACTTGCGTCTTACCCGCCATGTTGGCGCGGTGATGGGGCAGATGGTGTAGAATCGTGGATTAGTGAGATTCTGGGCGTCAAGGTGATAACCTCATGGGGAAGGTCTTACCTTCCGAGGAAAGATGCTATTGTGAGTATCATCACTTTCTGATAAGTGTAGGGCTATTCTAACCTCAGAGGCGGTGAATCCGCCTGTAGACGCCTCGTGGCGTGCCGCGTTGAACAGGACACTATCCTCCCAGTAATGGAAGTTGGAATATATCACTACAGCCTACCGAGTCAAGCTCTGCAAGCGCCGTTGCTCCAAAGCTCTTTTATCTGCTTTGCCCTTCAGCGGCTGCTAGTATTGGACCCGTGAAAGAATCTCCCAGCACACTAAAACAGTCCGGTAAGTGCCTTACGCAGGCGCCCGTCACCTGCCTATGAGGTTAAGTCAGCCGTGATATAATTCCGCGCCGGAATACTTAACCAGGGAATCGAATGTATCCAGTCGTCTTCACTGGACGAGTCTGCACTGGAGCGCATCTACCGACGCCACTTAGCAAGTGCAACATATGCATTAGAGGATCAATACGTGAGAAGGAAGCATTATGGAGGTTAAGCCTTCTGGGATTGTATGGCTACAGCACCGAAGCAGAACTTCAGTGGAGAGCTGATACTCTACCTCTTTAGACGGTATACTGGCTCTGAGAGACATAGATATTCAAGGGATGAGGTTTGGATGTGAACCCTTCCTGGGAAAGACAGGTGTCATCAAGGTTTCCTGAAACGGCCGCATAGCCAGAACGTCTGGGGGCTTGACCACTAACACTACTTATAGCACATCGCCGAGCCTGCTACACTGGCATGCGATCAAGTCCAGGATTACGCCCTAAGCCCTTAGCCGCTTAGAAGCCGGAGTTAGGAGTATTGACTAGCCATGAGCGACCATCGAGTTACGATTCTCACCTGTGTCTGTGTCTGCAGAAATGTCAGGTTGCGTACGATCGATAACAAGGTGCCAGCATAAGAACGCAAACTGTTTCCGTACCGACTAACTCATAAGATTCTAATGAATGTAGTTAGTGCTTATACATCACACATCGATGCAGTCCGCTCAGACTAGCGAATCCTGCTTTAGGGATGAGTAGCCCCTTAATGCACGTCTCAGCACTCATGTCACTAAACACACGCGGCGCGAACTTTAGCTCTAGCGATCGATTTTGGCCTCTGTGCCTATTCATGAATTCGTCACCGACACACTGCCCGCACGGCATAGAGTACGCCATAGAGTCTTTGTTCCAACTCTTTTAAGTGGCCATGCACTCGTTCTAAGGATAAGCTCGTAACCGTGTCCCATTTTCGAGCACACGGGCCTTCCTTATAATAGTATCCGGGCTGTTGTCCCGTGATGGATTGGTAGGTAATTTTTATCAAATCCGATGCCACTATCTGGAAGTTACCGGAACAACTCTGATCAGCCGTCTATAATGAAAATAGATGAGGACGGCGTACGACCCGGCCGATTGAAGAACTTCATAAGTCCAACTTTACCAAGAGTACATCGTTCACGTCATTCATCTTACACGACCTGCTCTAGGCATCGACCGCTGCCAACGACCTGAGATTCAGCCTTTTCATGTTGTTGTTGTGCGGTCGGTGAGTGGTCAAAACAAGGCCTCGGTCTAAACCCTCTACGAAGCTCGTGGCTTCAACTTCCTAGCGCACTAGTCATCGAGAGCAGATACATCTATTCTTCTAAGGGCATTGCGGGTCAGACCTTGTTTATGTCGAAATAGGGTTTGGTTAGCAAAACCGGAGACGGTTTATGAGAGGTCTTTGTGATTCAAAGAACTCGCAGCCTTGGCCGGTCCCGGGGACGAAGGGGGCTAATGCATTACACAAGCCTCATCCGACCAGTTTCGCATCGGGTTACTGTAAGGAGAACCAATAAAATGGAACTCCCTGGCATGGCTCGCGGAAACCCTCCCAAGCATGGGGCCGGACAACGCTCTTTGGTATAATCCCACACCGAACTTAGGGGTTTAAGCTCGCGGTGACTGTCATGTGTATTCGGAAAGCTAACTCGGCTATAAGTATGCACCTCATCTGGGAGCGTCCCGGTCGAAGACTTAATTCACGCATATGGTTAATCAGCAAGGCACTTGTCACCCAGTTGTGGGCCCCAGACGGAGTAACGGCAATCTGCGGATGCTTCGTAACCCGCCCAATTCGAACCGCTTGAAACTAACGTTCTAGTGTTAACTTGTACCGAAAGATCCACCATTATAGGCGTCGGGACTGTCGCGTGTACATGGCCTTTGGGCTCGTGTATGAGTGTCATACGGCAAGTGCGGGACAATCACTTCGATGGCTACGCGAACAAAGCTCGGCAAGTCCCGAGCGCGCTTTCTCATGCGAGGGTCTCTCATGCTTCACCATTACGTCGTCCACCACGTTGCCGCCCGGGATCGATTACCCAGTTGCGGTCCGAACCTCAGTCATACAATACCCCCGCTTGGCCTATACGCATTTAACTGGATATCAGTGAATGGAAAGGATCAGATTTCCGGGCCGCTCACGGTAGTCTTTCTGATCCCGTCACTTCCTACGATTTCCGCAAGGCATTCATTATATCTGTGATGTCTCACGGAATCTAGTCGTGCATGAGAAGTAAACCGTTTAGCATTCCATAGCAGGATGGCCCCTAGACTCTGTCACTGTCCCGTTTATCTCTGTGCTGAATACGGCCGTGTTGCCCATTAGTTATGACGCCAGACCACGTAGTTGTCATGGACGCCGTCATATGGTTCACTCGCCCCTGGGATGCCCCCCGGGCTACCCACATCGGGCCATCCGATCATCAGTTATTGGTTTCTCAGCCGATAGGCTTTCGACGGCGGTGCCTAGTCGGGTTGCAGCAACGACGCACATGCCGCTCGGGAACCACAGGCACGAGTAAAATTTACATTCCTATTAGGCTCGCGGCGGCTGAGCTCACTCGATCGGTGTCTCTATCTCGCCTTCGCGAGAAACACCAAGAGACATACATCATCCAGCTTGGGGAGACGCGGGTCGGATGTCCCATAATGCAGTCCAAGATGCACCTTCACGATAGAATTACTCCATAAGCGGAATTGGAGCTGTGCAGGTGCACGCAGCATGCTCACAAAGCTCACACACTAGTTCTGATACCGACCCCGAGTGATGGGATGTGATAAAGGTAATTGGGTTAGTCGAATTCTTTTCTTTCGAGGCGTTGCCGGCCTATCTAACTGAAATAATAAAGGTCTCTAGTCGGTCACCGCTAAAGGCGGCGGTATAGGACCAACTGGCGACCGTTTATAACTTGTTTCTGGTCACTTGTTTGAACCGTGCCTGCTGCTATTTGCTGTGAGACAGCAACGGAAAAGAGTGGTCAGTCCAGAGACCTACGAGATCGCTGATCAGTTACCGTGTAAAGTCGCGTCTACCCTTTGCCCGAGTGTGTATCCTGATGAGGTCTGGTCACATACTGAGGACAGCAATGAGACAGACTAGGTCACCGGTATTGACATATAAGAAAACCGATAATTCATTCAGTTGAGGCAGGTAATCTAGTTACAACTCTCTCCTTTCTAGGTGCAAGATATACGATGCCAGGCATTAGTGGAATTGTGTCTTTTGTTACTCACATATGAATCCTCTTTATTGAGTGTTCCAGATGGAGTGAAGATAACCAGTCCCAAAGCGAATTACATGACAGCCGTCATAAAACTGTGCCTCTGGGCGCATTGGGATCACGACGCACGTTAAAGCTCATACACGAACTAACAGTGCTTAAAATGGTCAGTAGCCAAAGTTTCACCTTTGAACCGCTATTTAATAAAATAGCAAGATGAGGTATACTACTCTGCGGTCCTAGATGCATACGGCCGGCATACAACCCGAAACGAGTCAATGTAGTCCAATGACTCACAATATGTAGCAAGAAGCGCCAGTTTGATCCTTACGGAAAGGTCAATCCACGGCACTGGTTTGCTGTAGGTCCGAACTCGATTCGGACCACTGCACCCGCCCCGGTAGGCGCCGGCGCGCCGGCAAGCTCAGCCACTTCTAACATGGTAGGGTAGATTGTGTAAGCGCGTGTTCTGCGACAGAGCGAGGGCATCTAGACATATGATATAGTAGACCATTGGCAGCATACACGGATGGTTCGGCGAGCGGCTCGCGAACGAATTGCTCCACATGCCACTGATTTGTCCTCCATGTCCTATGTCACGCAGATTTCACAGTAGACGCAAACGCGGCTCCGGCTTAGTTGAGGCCGTCACACGCCGTGGCAGATTTGTCTACGTAGGTTAGGATGGGCGCTGACTATGCCATAAAAGGGGGATTGCTAATACGCAGGGCCGCTCCTGGATTTCGGACTAGACGGATGATCACCAATGGACCATAATAGGTCCAGCCCTACAGATGTCGGGGGCCCTAGCGAAGGAACGCACAGTAAAAGTGTCTCGTCTTAAAGCGAATAGTCTCGTACCCCACGCTGTACCAAGTGCACACCCTCTGTTGGCCGGTTCACGCATGAGCTAATGATACCGGTGTCCTGCCTCATGTGTTATCGATGGGAGTACTGGCCGATTCTGTCGCCTAGATCCAGGCATAGGACGTTACTGTTAACAAAACTCGCGGAAGGGTTCTTTCGTCGGGGCGGCTATAGTTCTCACCGCGGGCTAAGGTTATGTAGGCAGCCTGTTCAGGGTCCCCATAATGGTATTTTGTCACGGTCCGCGACTTGACGACACTAAACGACGTGCAGACCATGACTGCAACGTCCGTGACTTACCTTTGTGTACTTCCGCGAAGTCTAACAGGCCTGAGGACTCAGTCGCCTAAGGCATAAGCGCTCTGTGTGCGGCGCTCCTTTCCGACTGACAACAGCCACCTAAATCTATTGTTTGCGATAGGACGGGGTCACCCATAGAGCTTCGGGTATTTGCAGCGCTATCCGTGGTGTAGAGGATCTCTCTTCATGCGCTTGCCATTTCTGCCAACGGGTTTGGTATCGTGCTGTCGTCTGAATTGCTCGGTCAGTATTCTGTCCGGCGGCGAAAGTTACTCTCAAATCCAAGCTTATTGTTGTTAATCCGTGGGTGTTAACTAAGTAGCAGGGTGCTGTCTTGAATATGATTGTACACGTTTATTCTTCTCAGACGGTCGGAGGACCTCTAGTTAACAAACTCGGGTTAGCTGCGGTCACGCGGTGGGTTGTATTCTAACTCTCTATATGGGCTTGCCCATTCGCACACTGTCAGTACTTTCTGCCTATGTTGGCGTTCAAACCAGATTGGGGAATAAGAGCTGAGGTAAAAGATCGAAATAACGAGGTATTCTTGTCCGTTTGTCAAGGGATCGCGCCCTTGCCTAAACTGGATACAGGATTCCTGGAAACGCCGCTCGGTTCGCGCACAGACTGTCTGCGTCCACTCGCCGGTGTGCATGTATCAATTTCCCCAATTCGGAATGCTGGTATTTGTCCAGCCTAAAAACGGATTGGCCCCGATACTCGCTCGTGTCCGCACGCGACGACCAGTTCAGCGAGAACGGGCAAAGGCGTACGCAATAATTTCGACCATCCCGTAACGTGATTGGCGCACATGTACAATACAGTCCCAAGAGATACTATAAGCACTCGAGTAGGCTGCCTAGACGCCGTATTCCTCCAAGCTTGCTCGTGAAAGATTCGCCTTAAAAGGTATGAAGGAAGACTGAGACCTGTCCCGCTCCGCGTCATCACGACGACTTACCAAGGGCTATGATCCTCACCGTTTGAGATCACGTTACTCCGCCCCTGTCGCGAGATGATCAACGTTGGCTTTGGAGTACCAGGCTTACAAGTATAATCGGACTTATTATTTAAGGGACGAGTTTCCATCGCGTACCAAGACTAATGGCATATGATACCAGGGATAGTAAGTGCGTATCCTTTTTAACGTACCGTTGAGCTTCTCACTAACAAGGACTCCATGTATTCTACCACCGAGCGCCTAGCGGACGTAGTTCGCTCAACGATGCCTAGAATAACTACGAGTTACACCTGGAGAACCTCCAGTCCCCGCGATTTCGCGATCTCAGCCGTATACCATGACATTAACTGTCATCGTTCAGCTTATCCTACGTTACCGGGGGTCTATTAACCTGATTTGTGCGAGTCCCACATGGCCGGATAGCCACCAGCTCAACATACCCCGTCCAGCTGTGTAGTCGGTCCGAGATCTGCGTTATGCTTTCTCTTAGTCAAGAGATAAGTAGTGTAACTTACTTTCTGTGCTCGGAAATCAGAAACTGGTCTCTGAGTAGGTCCTGAAGGTGGTTCGAATTAAGTTGATACCATTGGAGGAGCTAATCGTAGGCCATGTATCCCGCTGAGCGTGGGCTTTATGGCACGTAATTACAGAGGAGCAGTAGTACTCAAAGCAGCGACCCCGGTTAGTCTGTCGCCCTCTCGAGGAACAATTACGGCTGTAAGGGCGCGACAGTTCGTAGTACTGCGTCGGACAGTGACCAAGACCAAGACTTAACTGCAGTAGTGACCATTGTACGTGTCAATGGCGTCTAGATGCAGGTTGACCGTGATCCTAAATAGTCTTTACCAGTATCCGCGGATCACCCTGGTAAGAGCTTACGCGATAACCTAGCGATTACTATGTCTGGATGCGATGATTTCAGTCATGCCCGGGACGTCGGGAAAGCATGCGCTTATATGTCGCAGAGTTGATTTGCATTTGTAGTGTCCTCTATTCGTGGTACCAATCGAACCTAGATGTTCTGAGGTCTTAACTGATGCCTCTCAATCAACTTGCCTGATAGGCCGACGCAGTGTGCCCGATTGTGGTTAACGGTAGTTAAGCAGCGCGGCGAGAGGCACGATGGCCCATTACAATAAGCATCTGTAGAGTTCACGGACAACTAGTGTTGGAATGTGCCTCAGGGCTACAATCAGGTGCCTCTGCCACCCGACTAATAGCAACATCGTACCGCCCCCGGTTGGAGCCCAGGGCGGTGTCGGCGTCGTGGCACGTACGAAGACATTCTCTGGGAAGGATGGTCCAGCCACGCATGCGTTTTAGGCTACTGTCATAGGATAGTATATCGGTAATGCTACTAATTTTACCGACATGTTCATCGCTTCCGTTCAATAGTGGTCCATGAGTGCATTTTCCCCTCAAAGATTAGATTCCCATGTAGCCCTCCAATTCACTATCTAGGCGATATGGAGGGCGGGGATTCTTGACCGACACGACCAAGCTTCTCGTCCTCAGGTCCTAGTCCCCGCATATAGAACCCTGGACCGAGAAGTGATCGCGGCCGACTTTGTAGGCCGGGGGTTCATTTGAACTAGCTCGCCGATTATAGATGTGAGTGAACCCGGATATATCGCTCCGACTTAGCCCCGTGAGCCGAATCTACTTTTCTCTCAACGCACGTACGTGAGCTCCGGATTCTTTCTCACTTCCCTCGTCTAAATTGAGAGTCACGCCCATCTTAAACGCGAGCAGGCCGTCAAAGTCCACAGGCCCTAATTTTTCCCGCAGTCTGGAAGTGAGTGTTCTTATTTAGGTTTCAACCCTTCGATACATGTCGTTTCTCCTACCGAATGTGATGCGTGCGCTCCTTCGGGTCAGTATAAATGATGACGGACATTGGCACCTACTCTATTTTGATCTGTTGACAAACGTTATCCACTCGATGGACAAAGCGCACTCAACACTGCTGCCGGCACATTACACACTTCCTTTAGGTCCTCAATGCCGATCTATCTACAGGCCTGTGAGGGTTACGTGGGTACAGTGTGCTAGGAGTATGTCAAATCTGATCTACCTCGAACAGGAACCCAGAAAATAAAATTAGGGGCTCCTACTCCTAATCTATTCTGAGGGAACCCGTTTATGGCCAGTGCGCTAAATTATCCTGTGAGTCGAGTGAGCATGGCAGACGTTGACGCCAGGAACCATCTTGGCTGGAGTAATCCGGTTTCTCCATGCACACCCCCTAGTCATTCAGTCGGAAGCAGAATCTCCGTGGCACAAAGTAATTACAAGGACTACCTTTAGATCGGGAGGCCCGGCCATTCTCCTGTTGGACTGGAAGGTTCCTGTAATAGAAACTAAGATGAGTCGTCAACGCTAGGAGCTGATTGAGGATTAGTGCTGATACGGAAGCGTGGTTTGGGTATCGCAGAGTGGCTAGGATTATCCTACTCGGTGATGGACGGCACGAGCAGTGCCCCCGGATTCAGGTCCTTTGAACTTAAAACGCGCAATCCTAAAAGCTACACTTCAAAATCGTAGCTGGAACTAAACTTTACCCACGAAAATGCAGAATTTAATTCTCGAAGCGGCCTGGGTAGTGGGATTAGATTATCCATTTATGAGGTGTAATGCGCATTATCTTTGTGTCTGCTGTCCTGCATCACGAGCGGTTCGACTATTTTTGAACAGTGAGTGCTTTGCCATTGGGCACGCTTCCGTGTTGGTTTAAGAGATGGGTTCCAGTAGCTGCGCTAAAGGGGAAACATGCGCTACTATGCTCACCGCATAGGTTCGGTCGTTTGAGGATACCATCAGGCAAAACAGCGAGTCGCGACATCCCGACCGGCACGACGAATAGTGAGAACCTGTTCCCGTCAGGTGTCGATCGAGTTTCGCTATAAGTGAGCTTCGACCGCATGTGCACGGTCAGCCTTCCCAAACCATAGACAGCCCATTGTCATACTAACGGAATCTGTGGGGACTGCTGCGTGATTTAAGAGAACTGGGAAGAGCGAGGCATAGCAAAGACACAGATAATTTAAACTTCTGTCCCTTATGTCGGGAACCTGGTTATCAAATAGCGGTGGGCATACATGGAAGAGCAATTGTTTCGATATTGTTATGACGACGATGAATCGCGTGTGCGATGCCCGGGGATACCGTCGGAGCCTAACGCTTTAGATGATGAGATTAGTGACGGATCTTGCAGAGCGGTAATACTCAGTATTACTAGTGCCGCTCGATTCAGCAGTAAATTTTGAGGTCACCCGTGTCATTAAAGAAGCAACTTCTTTCCTCCCAACTTGTACGAAAATTAATACGAGATCCTTTACTTATCAAAACAAAGCGCCATTGTTTTGCAGATCCTCGCGGTTGGCCCAATGCCAGATCTGCTACGATCGGGTCTAGCGATACCATGGCGATCGCTTTGATGATCACTTATACTTAGGAAACGTCTTTATCATTCTAAAGGCATGTCGCATCTACAGTTACTGTATCTCAGGGAACTGCTCAGCACTGCAAACTTTGGGACAAGTCAGAGCCAGTTGTTGCGAGGTAAATTAGTGGTCCGAAGGAGTACGTGTTCCTCTTCGTTGTAAATTCAAAGGCATATCAATTGGAAAAAGCGGCCGCAGTTGTAACTGCGGTCTCAGCGCAACGCCGCCTTCCGAGTACCAGCACCCCTAGCTGTTCATTTGCCGAAATGTCCATGGGTCGTTAAACTGGGCTTAGTGAACCAAGGCATCAGTCGAGTCTCAGGCGAGATCCCTTCCTCATATGGTATGTGTGGCCTGACACATTCCTACACCCCGACGGGCGGGCCAACCTGGTAATTTTCGTCCTGCGGCCACCTGCGAATACGACATCGTGCATGGAGAGGCCTAGGCGCGTGTGGGTCCCCAAGTCACCATACTTTGATCCCCTTCGGATCTATTGCATGTTCAAGACGCAGCCTGCGCTCTCGGGACTCTAGTCGATCCGTTTATCATTTGTTGGGTGCAGCGGGGTTGAATTCCCTCCTAGTGTATCTTAATCAGTCGAATGGACCGCCTCCTTGGCTCTGACTCAGTTGACGGCTTAATCCTCAGTATAAGGGTAACGTCTTCTAGCAACAGACCTGCAGCCAATGAAGCCTCAAACCGGTATCCCTTCAGTATTAGTCTAACCCGTAGTTTATATAAATGCATAGCCGATCATTATATATAGTATGACGAAAGCGAAAAGAACCGGGAGGTATAGGGTTTCGTCATAAAGATGATAAGTTTGAGATACAGTCTAGCGCCGCCCGAAGACCCGATCGACCGGGGCGTCGGGCATAATTGAGTCAGCGTTTGCGGGCCTGATTAATGACCGAATGGATCTTCTCTCGTACTGTAACTTCTCTACAGTGTGTTTCAGCAGTGGATGCGAATAGGCAATTTGACAACTAGGTCAATATCTCGGTTGTTTCTATTAGGTGACCTACTGTGAAGAGCTAACTACCAAGCCGCGCTCCTCTTGGAGTGCTTTTGGTTTCCTAATGATGCTTTGGACAACCTTCTGCATGCGTAAAATCTGACAAAGAAGACATGAATAGGCATAGCCTCACTAAATTTGCGGACCTGGACCATCACTTGCGTTTCCACGCCCTATCGTTGATTGATGAGTGTTGGTCTAGTCCTGGCACCGCTCTTCCAATAGAGGCCCTTCGATGATGTCGTGTAGCCACCGACAGTTCATGGGGATCACTGCAAAGAATGTACACCGAGAAGAGTATATAAGTTAGAGCGAAAACAAGAGGCCCGTGCATGTTTATCTCGAGACCGTTGAAGCGAATTTCAGCTATGTCTTCGTCGTTTACTGTAATGGTCTTTAACCTGTTGAACTGAACACGATGGCTGGTACCACGGAATATGTTATGTCTCGTTCATATGCACTGCCAAGGTTGCATCCTCATGACTCTACTAGCACCAGCTTGGTTTAATTAAGGCGGGGGCTGCGGGCAATCGCGGCCCGCTATCGCGGTAAATTCCAGTACCAATGAGATGGTAACAACTAAAGTGCGAGATAGTATACGCTGGTTACGAAGCTGCAAGCGGTCGACAATGTGGGCAACTAATGACTACGAAAACAATATAGCGGGACCGGCGACGCTCCAATACCACGTACTTGGATGGGTTGAGAGATCGCTTCACAAATTGTGTACTGGCACTGTATTGGAAACTCGACATTGTGGAGGGTTGAAACCCTTCTAGTCAGACGTTTGCCTCGTGGATACGCTTGCACCAAGCATTACCGTTCCGGCCATGTCATAACATGCATGTGCGGACTTCGTTCGTTGTCAATGACTGACACCCCAGGAGGGATCTCACCGTCCGATACTGCGTAATTACTTAGAGTATTCAAACAGGGGTTGAGTACAGGCTACGGCGGGAGCGGTTGTCGTGAGCTCTTTGCCTTCTCCGCTATTTGAGATAACATGCCTAACCCTAGCGGAGACCTCATAGGCTAGTCCCCTCTCAAGAAACGATAATCTCAACCGACCGTACCACCACTTACGCTAGCGCTCCGAGCGCTATCTTAGGCAGACCGCTTCTCCGGCTCGCTAGATAGTTAGGGGGTCTACAAAGCTTTACCTACGTTAATCAACACTTAGTGAGCAGCGGGACATTTTGTATTAGAACAAGGCCATTGGACTAAGATCAGTAGTGAATCCGTATACGTCCGGCCCAGCGCTCCATGTGTTGTTCCAGTCAGAGTAAGTATCACAGGCCAATTCACCGGCGTCCCCAAACTGGCCCCCCCTCTTGAACTCAGGCGGCCAGCTTTGGTTACCGAAGTACACGCACAGGTTTGCTGGAGAGAGCTCAATCCAAGTTGCAACGCTACCGTTGAACACACCTCCTCATGCCTACTATGGTGAACTTTTCTAGCTAGACTAATCAAATAAACAATCTGCAAGCACCAGCCGAGAGGTGGGCGATGGACATGTTCTCTTCTTATCGGTCCGTGCCATTCACATACAGTACCCGGCTGGCGACGGTACATATAGTTGATTGTGGGTACTGCTCGCATCATCCGACACTAGGTTATACTCTTCTTTGTCGGAATGACCCTATATCGATTCGATGGGGTAGGTGGCAGCATGCCCGGCTATTAGCAAATGGGAATCATGAACCTGGGACCCTACCGGATTGAGGTGGCAGATCTGTAGTCTTATGTCTTAATTTTCGAAGCATTCCAGGGTAAGCGACGCACCCCGTAAGATCTGGACCTAGTGACTTATCCCCCACCATGTTCGAACTATGTTGAAATAGCTGTTGGGCTCAATAATTGCGCTTTTCTAATGAGATACACACGCTCTCCTCAGGACCAGCTAGGAATCGCTGTCCTCCGCAGAAGCATTCGGCAGCACTTAAAGACAACGATGATTGCCAAAAGTACTCCAGCGCTGTGGTTTTTTACGCGCTGTTCTGGCCAGTCTTTGACTAAACGCCAACGCAATAGAAAGCCACGGGTGTTTCTGACAAAAGTCATAGACTAGCTTCACATCTGCCTCTGGAGTAAAGCCTCAACCTATCGTTTTTCTCAAATCATCCTCCGCGTTGCGATGCAGCAGCTAAAAAACAAGCACGATAAGACTAGATGGTATAGACACGATCACTATACCTCGATCGTACGTATCACATGCGGGAGCAGTTATATCTGGGGCGCCAACGGTGCTGGACTTCACCAATTCAAAGTTGGTATGTGATCCTCGTCACGATGGATATTTTGGCCGTCGGCAGCTTGGCGGCGGAAAAGACCGCGGCCAGTTGAGTTTACGAACCTCAAGAGGGCCGACCGCATATATACGCAATTATCCTTTCAGCGAAACCTATAAGTGCGCCGTTGACTCATTTGTGTGACAAGAACTCATCGTAGAACCCGCTGTAGCCTCGCTGGCTCGTGGGTGGATTATGGGACAGCTTGCGGTGACACTGACAAGATATTAATCCACCGTCAACAGGCAGAACCCATATATAGGGTAGACCAATGTCATCTTCAACCGGTAACCTGCCACCCATGTTGCTTGGCCTCCAGTTTTCATAAGACGCAGGTCTTTATGCCAAACTGTACGGATCATCATGATTCGCGGATAACGGACCAGGCTGCTGAGCTAGGGGTATGCAGTCCCTGTACTGATTATACGTGCAATGAACTCGAAATACCACTCTGACGGCTGACTGGTCCTCCACAACGAATATAGACGATCGGCATGAGCTCTAGAGAAAGGGTCGAATCTGCCAGGCACGTATCATAAATCTCTTCTTTCTTTAAGCCGGTTGGGTGTATGAGAAGACTTCGGGCGTCACAATACCTGGCGCCGTACCCCATACGTCTTTACTGACGAGCCGAAACGCTATGGCAGGCATCTTGCCACGGAGCCTATAGCAAGCCCCTAAGAGGCGGGATAAATCATGGACTTGAACCCTGTCGCAGTACGTAACCATGGTCTAATCCCTCGGTTTAGCCAAGTAGCTGGGCGACATTGCCATTAAAGCTTCCCTTAAGCGATGAATATCACTCTGTGCCCCTATCGCGGCTCACTACTTTCTGAACTGCGATAGAATGCCTCTCTATCTACCCTACACACACAACTGCGCTGGTGCCCCACGTGTAATTTTGATAAGCACTACTTAACCCGGCCAACCGATATGTTGAAGTTACATCTATCTGGGGGTGGTGCACCAGAAGACCGTATGTGATTTATGGGGGTCACACGTCACGAAAAACATTTCCCCCGGCCAGTGGTGCAGCCACTACTATACCCTATTGAGTCTATAAATGCCAAAGGCAGTAATGCGTCTACCTACACCGTGGCCTGTTCGACCGACCCCACGTAAAGTAATCTGACTCAACGCTGCTCCACTAAATTCGGAAACATATCAGCGTTGATGACAACGGGTTCGAGAGCATACATCTACCAGGCATCTCAGTCCTTAGTACTTGATGCTTGTATGGGCTTATGACTGGACATTTGGGTATGCCCTCCTTATCGATGATGGTTGATGACCCATGGTGAATGGTTAGATCGCGGGGAAGGTTGAGCCATCTCGATAAAGACCGGCTTGGGAGTGCGAATCGATTATACAGAAGGTATAGCAAGGGTGTAGCGCAGACACAAAGCGGTTCACAGCTTTTCTGTTGGAGATTTTAGCATTGTACGAGTCAGAAAGCAACAGCAGTAAGTGTCGTACAACGAGTCCACCATACCGAAGAACGTAACGTCATCGGGCGTTGGGATAGAGGGTCCATCACGGCCCGGAAAACGTCGTCCTGCAAATAAGAGCGTCGTATTGATGTATAAGAGAGCGCGTAGGCTACAGAGCAACAGTCTGACACAAGACTATGAGCTCACATCAAAGTATTACACCTCTTCTAGCCTTGGCAATACTTCGGCATTCGGTAGCCTATGGTCATTGATGCTGCTTCCGCATCCGGGATTTACTGTCTCGATCTCTGTATTTTTCACGTCACGGGTGGGTATAGAAGCGCTGGACCGCCACCCCTGAGGGGATGTGTGCGCGAGGGGTACGCTGGAATCTTGGCCAAAAAGCGCGGGTAAGGGCCGCGGACTCATCTAGTGGTGCCTAGAGCTTCATGATTGAGAGAAGTAGTCCTACATTCACGCCCCGACTTTGGGTCCATCCCAGGAACCCTTAGATTTGGCTAATGGTGGAACGCCCCGAGTAGCCAGGAAGGTGTCCCATCAAAAACGCGAATTCAACGCGAGGTTGCTTCTTAAATGTTCGTCTCGTATGCTCAGACCGACCATCGCCTACCGTGGAAGGCTAGTGGCCGTACTGGGTGACAGAACAAAGGTGGGTTCACCGTGGGCCTGGGTTCGGGTGTACGACGTTGTACTAGTCTATCCGCCCTACTAGTGACATACAGGCAAAATCTACCCAGATTACTGTGAACCGATGACTAGCACCGAGCCTGTTCGTGTTCACAGGAACGGTCTCGATATGCCCCAATCCCGCCATTATCCAATATGATAAGGAGTGACCCTCTATCCTCAGATAAGGTCGCGCGTTGTAACCAGGTCGAACCCTATACTTACTACTTGGCTGCCTTAGGACAGTTGTGTGTATTTGTAGCTCAGCTTTTATTACATCATGAATCACTTTGAAAAACCATATGTTGCCGTTCCTCACGTGTCAACACTGGTTTTTCCTCACCCAGAACGCTGTGCTCTACAGGCGCAGAGGTATGAGGACTGGCGCGACTTAGTTCGCGGTTCATCTGCAATTATACGTAGGGTATACACAGATCTTCTCGGGAAAAAGAGAATTCTCGCTGTCGTCGCATTGGGGACTCAAGTGTGGCCTCCGACCACATTAAAGTGATGGCGGTAAGCTGTTAAGCAACGAGAGCAAGTTAATTTCAGTGGCATCCACCAATTAGTCATAGCATGCGTGTTCAATATATGTTCTCGAACACAACTCTGCAGGGACGACGAATGAGAGTGTGTTAACTCACAAAATGTGCTCAATAAAACCTCTTACTTACCAATTGAACTAATTATAGAATATGGATTTGTTTGTTCCTGTTAAGTATCCCTCTAGCAGAGCCAAGCTGCGTTACCTTTACTAAAGTTGACATGAAACATGCACGGGTGGCGTCCAGCCTTGCGCAACCTGGATGAAGCGTAACTAGGGTAAAACAACGTACATGAGGGATGAGATTTAATAGTCTCTGTGCCATAAGGAGCTTGGTATTTAAGATCGCTCAGGATGTGGTGTCCGGAGCCATCGGCGTGCCGAAGGTCGACGCTCATCATTATTCATTTTTGAATCTGCTTGACTGTTTGCGCACACCAGTGCTTGCCATCGCATAATGGGAATGTGTGACTGAGTTTTGGTCGCATGTGATTGTCAGCCCTACGCTTCGAGGCGCATTGCTAGCGTCGACAACTCGGCGCGGTGTTACGCTTTCAAGCGCTCAAATTGCGAATTCCTAGCTACTCGTTATGCCGTAAAGGCTCACGAATACCAGAGCCAACGAGACTACTATTCTCAAAGTGCATACTCACTAGAAGACATAACGGTCATATGTTGGTACGGTCGCTGAAGGTGTCTAGGAAAGTCATCGTCAGTACGTTTAAATTGCAATATACCCTACGCAATATATAGCAATTGAGTTGAGCGCGATTGTTTGTGAACGGTGGCGGTGCAGATTAGCTCGTAGGCCCGTGGTATAAAATCGGAGCGCCGGTGGAGTCGCCGCGAGCGAGGTACCCATAGGACGATTTAGAAATAGGGGGAGACCTCTTTGATCGCCTGGTAATCGTAATTAGCCTACCAAGTGTGCAAATGTGATCAGTGGACAGCTACATTCTATCGCCATAGTGGAGCGTGATCCTTAAACAACACGCAAGTAACAGATAACACGTCGTCCCTGCGCGATTAAGAGAGATGATAATATCCGAAAATGTGGAGTGGGCTTAACAGTCCGAATGATCCTAATAATGCGACCGCCTCTTAATGGACAAATTAAGTCAAGGTCATCTCCACGCATGGCCCCTTCAACGGATTGATTGACGCTACCGCGTCTTGTCCAGGGAAAGTCGGGTGAACTCGACGAGTGAACCTGTGTTCTGCAGTACAAGCCTGCAGCGTTAACATACGGTGCCCGAGTTAAATCGACGGCCCATGGCGACACTAAGTGTACCTGATTCTTGTTCAGGTGAGGCGGTATGCGGTTAATTACGACAATTACGCGGCACCAGTCGCCGGCTATTCGTGATTCACTCGGGATGCGCACACTTGTGTAGTTTATCTGCACTGCGCGTCGGTACTGCGCTTTGGCGGAGTCTGCGCCAGAGACACTGGACTGCCTTCTGACCCCTACCGATATGCACCACTAACGGATGATGCCGACATCGACCATCACACAAGGTAATCATCACTACTGGAGGGAGGGTTTGGGTTCCGCTTATCGCGAGCAACTGTATGACTGAAGACCTCATGCAGAAATACTCTCTATGGCATCCAACTTTGTCGAGGACACTAAATTAGTCGTTTACAAGCCACACCGTCACGCCGCGCAATCGAGGATTGTACATGTGAATATATTTTAGGCCCCGTCATCTCGGCTTGACGGTTTCCAACGCGGACCCCCGAATCTCTTCGCATCCCGCACTGTGTCCACGTCAGCAAGTGGTAGCCACTGATGTCCTCACCCTTCGAGCTGATCCGTAATCATGTATAGAGTGCGTAGATTACCTAGTTCGCTTTAGTTCGTTGGAAAGCAACTCCGCAGCATTCTATCTCGTGGGTATTGAAAGATTTCGTTTTACGCTAATTGAGTACGCTGTCCTGTTGGCTTCTCAAGGTGGTCTACCTAAGGGCTTTCCCTGATCTTTCAAACCGACTCCCCGAACGAAAACCACAGATTCTGTCAGTATGCCCGTGTGAATTCCCATAAACCTTGCAGAAAGCGCGTCGCAATCATACTGCTATGGTAGTGTATCAGGAACTAGTGTGCTATTGCCGCGGGATAGCCTAGGAAACGACGTACAGTACTGCTCTGGCACCCATGAATTGCAAATTGGAAGTAATTAGACATAGTACTCCGGAGTTCAGCGGTTCTCTTTATTAACTAAATGGAAAAAGGGGGTTAAAAGATACTTTCTGGCCTAGTCACATAATGGCGGAACACTATGGGATCAACGCCCACAAGACAGTGGGGTTTTTGATCCGTGATGTCTTAAGTAGTAAAGGCGACACATCATAGACTGCGGGGGCCTAAAGCTCCCAATCTCACGGCTACTGCCGTCTTTGTCTTCACAGCCTTAGACATGTAATTGCGGTGTGTGGTGACTTAAAGAAGGAGACGATACATCCTGGTAACCTCCGGAAGGTGATATATGTGTAAAACTGGCTACGTTCTTAGCCGGGCGAGTCCCTGACTTCTAACGGCAAACATAACGACGCTGCTTATGGCTCTTGGGCATACTGGACATACCTCAGCCTAGGCAACCTTCCATGGCTCACCGTCGGCCTGAATCACTCAGGCCCCACTGAGGCGCTGGGATATTGTCATGGGGAGGGATTAGCAGTATGTAAGAACGAAATCGGACTAACATTGGGGCAGGACGCTGAGCCTGAGTCATCACTTACGAAGGCGCGTAGCGTTACTCCCTACATAAGTATGGAACCTTAGAAGACAGAGCCGCCTCACTAGTTCGCCCATTTAAGGATGGAGAATTTATGCATGACTGGTGCGAGTATTTGGCCCACTGTTTCTCCTTGGCCTCAAGTCTTTGAGGTCTCACGTCGAACGTAATTCCCGGTGTCCCGATCGAATTACCAGGCGCTGCCATTAAGCACGCGTGATCTCGAATGCTTCTACATAAACGCGTTATGGCTTTCAGCGGGATTTGAGACCACGGTATCTTTACAAACGCCCAAGCCAAATAATAGAGCTTTGCCAGTGTGAGCCTAACAGGGCACTCTCTTTGATGCAACCCCAACGTTCCGCGGGGTGATCTTTCTCTGTAACGCACCGTTTAATATGGAAGTCTTTCTCTCATAATTCGTGGATCGCAAATATGTTAAGTACGTTCGTCCACGGTCGCATTTCGCTCACACAAGCTAAGAGGGGATGATTTTTGTTGTTGCGTACATCGTGTCAAGCGGTGGCCAAACGGGTACGCGTTGTACAATGGCCTAGAATGCTGGATCTGGAAGAATGGTTGGACTAATCCGTCGCTTGGTCTGGTTATCCGGAACTCGCACAGTAGAGTCGGCAGGGCAGCTATTCCACGGAAATAACATTAGGTAGATTCGGAAAATAGGTACATTACGCAAGCGGCCGGGCTTTATTCATGCGAACGGCTCCATATGAGCGAGCTAGCACTTGGGCAATGCCGAGAAAACTCACTTTCAGAAATGCCGGCCGTTAACCTTGAACGAGCTAATGTTAATGCCATGTTCGCCGGCAATAGGGACAGTAGCTCCCACAACACGTTGGCCGGAATCTCCCGTATCTCGAATCTAGAACGAGACAAATTTGAAATTCGTAATGCGGTCCAAACTGTCTGATTTCATAGGGCAGTTGTCTCCGCTTAAAAATCACGATAGTTTCTCCCTGCCTGCAGTATAAGAGGATCACCGTCTGACGCCCTCTCCATTATACAGTTTATGGACGGCGCTTGAACTATCTCCGCCCTTTTCGCTCGTCTACTCGCGCGGCACGATCCAATTCATCCTTGTTGCGGGGAACTTGTCCGCGATTTGACGGAGCCTCGTGGTAGCGAAGTGGAAGATTCGCGATTATCACACTAATGAAAAAAGTTTTCGTGAAGAGTGTGACCGTCTGTGGCAATTGCTCTTTCTATTCGAACAAGTCCGGTATAATAGCGTTCGGCACAAGTACAATTCCGATGCGAATGGTTGACATTGCTGGCCTGCCTCCTCCTATCGCGCGATGTTCCTACAAGTTAAATATAACTATGCCTGACCGGCCGCAGAAGAAAAGGCACATATCCTATTATCAGTTCGCCTTATATCGCAGCCGAAAAGTCCGCCTCTTGATTAGCCTTCTTTTGCTTGGCAACAGGGCTCTAAGGGAAAATTCCATGCCTGGACTTCCGATTAGTGTCCACCCGTTATGTAAAAGCCTAGTAACCTTAACCTACGGTCGGAAAGGGACTCAGTTTGCCCTCAGGCTTACTTTAGTGATGCATTGGGGCGTGACATCAGCGCCAGTGAGTAAGATTTTGGCGTTTAGCAGCGTACGCCACCCGCTCTCGTTTGCCCGCGAACGCATACTTCTTTGTGGCATGAGCCAAGTGGGAAGCTCGACAGCCGTACTCATTCAAGCATGTAACTGATCATAAAAGCCATTTATTGGCTGTCTTGCAGAAGGAGGACCGAGGGAGATCTGACAACGAGAGACGCACAGCGCCCCCTGACCCCATGGGGACCCCAGGGATATACTTCGACGCATTCAGTTTTGCGTGACCCGCGGTGTGCCAGGCGGAGGGCCCTGAGCGTGATGCCTGATGGCCCTGGTATCGACGAGCCACGGCTACGAATGTTGGCTATCATGTTCATACGGTAAGGAAAGAGGAGGACTTAACTTCATGCGTTTCGTAGCGTTGTCCAATCGACTTGCTTGCTCCTTGCGTAAATCCCACAAAACGGCACGGTTGCCGGTGTGTCAAAATGGGAGTAAGTTGTCCCTCAGGAGATCGTAGCTAAATATCAATGGAAATTCCTTCTCTGATCGCCCAACCGTGAGTTTAGGTCCCTTGGAGACGCAGGCTTCGCGAGTCACCCATCTCCCATCCCCACCTACCGCAACGTATGGGCAGCCCTGTGACTTATTGTATCACCATGCTTTGCTTGACAAAGTAGGGTCTATTTTCCATCATACGGCCCTGACTGCATGTACCCTTAATCCCGGTAAGAAGTGAACCCCTCGGTTGGTCATTTGTCGTTGGACTAGAACTTTTCAGCGTCGCAAACTTACACGGACCCGACGCCTGAACTCCCTTCTGATACAGGCCGGAGTAGTAGTACTTATCATCCACTTGTCCTCATTGTTACTGTGTGTCGCGGGTCGCTAACGAACCCTAACAGTCCTCCCACAGACTTTCGGGTTAGACTAGACGTTGGCTCGACTCCATACGGTGGACGGTACCGAGCAATGGTAGACTGTACAGCACCACCGAATTTTCATAAAGCCCTTGGTCGACATCGGTGAATGGACCCAGAAACTAAACTACGATAGTTAATTTGGGTCTTCCGGACACTAAGAGCTGCTCCTATGAACCGAGCTATAAACCCGCCCTACGCAGGACAAAGAAAGAGGTCGATTTTCGGTGAGCGTCGACCGCGTGGAGTCATCCTTGCTGGGGCCGCGACATGAGGCGCTTGGCAGAATCCATCAACTGGCGTCCGGGAGATGAGGCTCACTCCCTGATAGGGGTAGCATCTCCGACATGATCACGTAAATGTGATGTGTCAGCATGTCGAACCCCTACTCCCTGTGCGCAGTCCGTAATTCGGGGGCAAAGTGTACATCCAACTGAGATCGCCTATAGCCGGAACACAAAGGCGCGTGGAACGCTGTCGAAGTCCTTAAAGAACGTTTCTCTACGAGGACGTTATGATAAGAAGGTGCCCCCAACTGAGGGATCGGAGCAAGACACAAGAAAGGTGCTCGGTGTTCAATGGACTGAGCAGTCTTAAGTGCCGACCCGCTTGGTCATGGATGCTTCTGAGCAAATTGCGGGCATCACGAAGGTTCTCCAGTCGCGTAGTAAAGAATATGTGAGTGCTATCACGCTACGTGCTGGTCCGTCATGAGAAGAATTGACTATCCACTGTCGTTAGCTCAATCTATGTGGATATACTCTGGAAACTTGCATAAATATCTTGAATCGTAGGTAGACACCAGTGATTGGAATTGACAGCTTTACTAAGAACATCTGCTTGGTTCTGAGTCAGGGTTTTAAGTAAAGAAGGGAGGGGTTGGCTGGACAGAGTGAATACGTTGCACTCGAGCAAGGCAACACAAATGAGGACTTAGGTGGGTGATATCCCTTGCTCACCTAACGGCGCCTACGTAAATGCGATACCGTGTTACCAGCAGGCTGCCTTCGCTTTCGCGGCTGGTTTAATATAAAATCATGAAAACGACCTCTACTGAGTGTATATACCAATGGGACAATTGTTGTATAGATTTAGATTTGTACTAGCATGGAACGTCACAGCCGAGTCGCAGGGACTATGAATGACGGTCCTCACTTAGCCTTCCAAGCGGGTTGAAGAGCAGCGGCAAGACGAGGTCGCGTTCGGTTGCTATAATTCACAAGGAGTGCCGTTGTCAGAACCTTAGGCTTTACGAGTCCTAAGTAAGATTATAGAAGTTGGTACGACCTTGATAAACCACAGTTCTGTCTCGGAGCTACGTGATTCTATGCAGTACGCAGTCAGCACTTGTCTCTTGCCCAGTTTACGAGTAGCTTAATCTTTATCGTGGTCAACGGCCCCCTGTGCCCAGGATAAGTGACCGAATTTATGTCCAGTTCCTAGAGGCAGATTTCCTAAAGAGAGTACGAGACTTTAAGCCCGTACCAAGGACTCAAGCGACTAAAGCCTCGCTACCGACGAGTGGTATGGGGGCGTTAGTATCGTTACATTCCGGCACGGATAAGCGTATTAGCAACCTTTTTTATATCTAGGGTTTGAATTGGGAGTGGCTAATTTTAACGTGGACTTCTAGACCAAACGGGATCTATCATACATCTTCCTGGACAGTTGCAATGGCAGGCGGCAGGCTTGGGCAGGGGTGAGTCTGTCATACAGCACGAAGAATAGGAGGGGCCTGGCTCTTGTCCCCAGTGTACTCGTATAGGCGAATACGCGGGGCTTTGATGCGTGGCGTAAGAGCTCAGAAATAGGTGGTGATACAATGTGTAGTGAAGGACCCAGGGTTAGCCCCCTACGGTTATTGTGTGGCGGTCGTGGCGCCCGTGCTGCCCTTGGCCAACCGTCACCATTCTTTACGAGGTTCATGAGGCCCACTCAATACCGCGAGGGTTACTAAAAGACTGTCTAACAATACATGGCTAAAGTCCCAAGGGCGATGCGTGATGGGTATACGAGACCACTGAAAGAGCCTTTATTGAAATCGAGCAGCGGAATCATGCTATCCATTCTAAAAGGAGAACACAGGGCCGGATATGTGTATATAAGGACCTACGTGTCATGGTGTGAGAAGCCGGTCCTGCATGCCCAGGTAGGGACCGGGATACACTACGCTATCTTCTGTTAATCGGATTGAATCTGTTTGCCGCGTCCAGACCCGAAGGCTCTGGCAGCACTGTCTGGCTCACACGACTACGCTTCCGATTCCGGAGTTGGCAGTTAATTATGTTTTGGCCAAGAACGTATTCCTGTCTTGCGTAAGAGGAAACGAACCTATGTGTCGCGCTTGACATGCTGCCGCTGAGAAATGGGGAGCGTTAACTTCAGTAGCCTATCTCGGGAACAGTGGACCTCATCATACCTTCGGCTTATTAGACACTACACGAAGTAACCGCGTCGACCCAATCCGTCTCAGACCGTCTATGAGGTCCGCCATGCCTCCATCAAAATGGAAACTTACATGGGGTCGTGGGGATAGCTATCAAAACGTAGGCCCTCGATAACGCTGTTGAAAGATAACGCGAGGGTCCTCTCTTGGAGCAACGATCTCAACGCCGACACCAATCGGGGGCTACACGACCCGTTTCGCTTGGAAGTGGCAACTCGAGTTATTCGCGGCAACTGTGAGTTTCAAAGAATGCGCTCTAGGTCCTTAGCAGTGATTATCCCGACAATCCCAGGAGTCGTGGTTGCTCGCACTAGCCGCCCGGACTGATCACGCGGCCCCATCCACGCCTTAGCTTAATAACCAATCAAGGCTTAAATACAGTCTGTGTAACCAGAACAACGACATGTTAGAGAGGACTCCTCCCTAGACAGAAGAAGTATGGTAGCCCCCTGCGGAAGTCAGGCCGGGCAACTAGTGCCACGCAAGCCGCCAATCTGTGAGTGTGCGGACGATGGTGAATCGGCTCAGGCCACCAATGAGGTAGCGAAACAAACTCTCAAAATCTCGCAATTGGCGACTAACAAACATCGAAGTGCGGCCCGAACGTGTTTGAATGAGGTGAATTACAACCTGCATTGGACAGATTGGACTGCGTCCAAGTCAAAACCTCAAACCCTCCAAGCGCCAATTATCGAGGCCTATCTTTTTCATAACCCTTCACTTAATTGGTGGGCGTTGGATAGACTCAATTTAGCACTCGTGTCTGCGCAGGCTGTCATTTGGCGTGGGGGTATCGTCCAAAGCGCGACAGAAGGCGCAGTTGGCTGAGGAGAAATTGATCGCCGTTTTCTTTCTACCTACTGGGTGCTGGTGAGCTTGTGCAATTGCTTGCTGATTGTTCTAAGGGTCGATTCACTGAGTATGGAGTCATGGGAGACGACAAAGTAACACCATATGGGTCATAGAAGGCGTAACTCATTCACTAGCGGCAGCATCAACCGATCTCCTTGGTGTTAAGTGGGCATCCTTAGGGGCAGGCGGAGAGTCACCAAGCTCTAGTAGTGGCCCACCAATCGCGGGCTTAGAGTAAGCTTTAAGCGAGGGGACAAGACCTTCAAACCTTGCGCCCACGGATCGCTTATTGGTTAAGCCACGGACTTGTGTGACCACAACCTATTAGTCTCTTCGTGCTTAGTTTAGGTTGCTATTGAGAGCAACCTCGGTGTTTAACGTCCAGATTCTTACAAATTTGTGGGGTACCATCCCGACAGGGGAGTGGTTTAAGGTTCACAAATATATACTCTAATATTGTGGTCCTAAGTCTGCAGTGCTTTATCTTTACCTTGAAAGTCAGCCATCTTAGCAAGCAATGCGTTAGGAACCTAGAGCCGGATGGTGTCGGAAGAATCATGTAAAGTCAGACGCGTTATCGATTGTACTACATCTCCTCCCAAGAATAACAGCGATCAAGCACATTTGTGCCGGACTA";
	string s3 (10000, 'A');
	vector<string> ans;

	ans = findRepeatedDnaSequences(s1);
	for (int i = 0; i < ans.size(); i++)
		cout << ans[i] << endl;
	cout << endl;

	ans = findRepeatedDnaSequences(s2);
	for (int i = 0; i < ans.size(); i++)
		cout << ans[i] << endl;
	cout << endl;

	ans = findRepeatedDnaSequences(s3);
	for (int i = 0; i < ans.size(); i++)
		cout << ans[i] << endl;
	cout << endl;
	return 0;
}
