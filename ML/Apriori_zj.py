#FileName:Apriori.py

import sys
import copy
import os
import itertools
import pdb
#actors id sorted
def InsertToSortedList(tlist,item):

	i = 0

	while i < len(tlist):

		if item < tlist[i]:

			break
		elif item == tlist[i]:
			return

		i+=1

	tlist.insert(i, item)



def CompItemSet(x,y):
	i = 0
	while i < len(x) and i < len(y):
		if x[i] < y[i]:
			return -1
		elif x[i] > y[i]:
			return 1
		i += 1
	if i == len(x) and i == len(y):
		return 0
	elif i < len(y):
		return -1
	return 1

RawFile = "casts.lists.txt"
ResultFile = "casts.fis.txt"

infile = file(RawFile,'r')

s = infile.readline().lower()

WordIDTable = {} #{'zhangjie' : 1, 'wanghao' : 2, ...}

WordList = [] #['zhangjie', 'wanghao', ...]

WordFreqSet = {} #{1 : 1, 2: 4}

TSet = [] #Transaction Database

ItemSet = [] #A transaction [1,2,3,4,5,6,7,8]

# load transactions
while len(s) > 0:

	items = s.strip().split('\t')
	# items[0] is filmName
	# others are actors
	for i in range(1, len(items)):
		tmpstr = items[i].strip()
		if tmpstr not in WordIDTable:
			WordList.append(tmpstr)
			WordIDTable[tmpstr] = len(WordList)
			WordFreqSet[WordIDTable[tmpstr]] = 1

		else:
			WordFreqSet[WordIDTable[tmpstr]] += 1

		InsertToSortedList(ItemSet,WordIDTable[tmpstr])

	TSet.append(ItemSet)
	ItemSet = []

	s = infile.readline().lower()
infile.close()

print len(WordList), "person names loaded!"
print len(TSet), "transactions loaded!"

#ItemSetComp = lambda x,y:CompItemSet(x,y)
TSet.sort(CompItemSet)

MinSupCount = 5     # set the minimum support
LSet = []           # frequent item set list of k-frequent list, index by k(0,1,2,3,4), 0 is 1-frequent
CSet = []           # candidate item set


CSet.append([])

# get 1-frequent item set
LSet.append([])

for (item,freq) in WordFreqSet.items():
	if freq >= MinSupCount:
		LSet[0].append([item])


LSet[0].sort(CompItemSet)
print len(LSet[0]), "1-frequent item sets found!"

# remove transactions containing no 1-frequent item set
# and get 2-frequent item set
Freq2Dic = {}
for itemset in TSet:
	i = 0
	while i < len(itemset):
		if WordFreqSet[itemset[i]] < MinSupCount:
			itemset.remove(itemset[i])
		i += 1
	if len(itemset) < 1:
		TSet.remove(itemset)
	elif len(itemset) > 1:
                # generate the dictionary of 2-item pairs, calculate the frequency
		for j in range(len(itemset)-1):
			for k in range(j+1,len(itemset)):
				temps = str(itemset[j])+'-'+str(itemset[k])
				if temps not in Freq2Dic:
					Freq2Dic[temps] = 1
				else:
					Freq2Dic[temps] += 1
# Get 2-frequent item set

CSet.append([])

LSet.append([])

for (item,freq) in Freq2Dic.items():

	if freq >= MinSupCount:

		templist = []
		parts = item.split('-')
		templist.append(int(parts[0]))
		templist.append(int(parts[1]))
		LSet[1].append(templist)


LSet[1].sort(CompItemSet)
print len(TSet), "transactions after pruning!"


#new LSet has 1-frequency and 2-frequency
def IsEqual(list1, list2):
	i = 0
	while i < len(list1) and i < len(list2):
		if list1[i] != list2[i]:
			return False
		i += 1
	if i == len(list1) and i == len(list2):
		return True
	else:
		return False

###################################
# 1: You need to decide whether 'newSet' is included in the candidate item sets for (k+1)
# 'tmpDic' is the dictionary built from k-frequent item sets
def IsValid(newSet, tmpDic, k):
        # TODO:
        Combinations = list(itertools.combinations(newSet, k))
        flag =True
        for i in range(len(Combinations)):
            item = sorted(list(Combinations[i]))
            if item not in tmpDic:
                flag = False
                break
        return flag





	return True

# link and prune

def GenCand(k, LSet, CSet):
    # generate the dictionary built from k-frequent item sets
	#PreSetDic = {}
    PrevAndSuffixDict = {}
    for itemset in LSet[k-1]:
		#s = ""
		#for j in range(len(itemset)):
		#	s += "-" + str(itemset[j])
		#temps = s[1:]
		#if temps not in PreSetDic:
		#	PreSetDic[temps] = True
		#else:
		#	print "Duplicate frequent itemset found!"
        prev = tuple(itemset[:(k-1)])
        suffix = itemset[(k-1):]
        if prev not in PrevAndSuffixDict:
            PrevAndSuffixDict[prev] = []
        PrevAndSuffixDict[prev] += suffix
        PrevAndSuffixDict[prev] = list(set(PrevAndSuffixDict[prev]))
        print(PrevAndSuffixDict)
    #PreKPlusFreq = []
    for key in PrevAndSuffixDict:
        item = []
        value = PrevAndSuffixDict[key]
        Combinations = list(itertools.combinations(value,2))
        if not len(Combinations):
            continue
        for i in range(len(Combinations)):
            item = sorted(list(key) + list(Combinations[i]))
            #CSet[k].append(item)
            if IsValid(item, LSet[k - 1], k) :
                #LSet[k].append(item)
                CSet[k].append(item)
	###################################
	# 2: You need to generate the candidate item sets for (k+1)
	# You MAY call the function 'IsValid(,)' you have built, and use the dictionary 'PreSetDic' generated above

	# TODO:



















def GetFreq(candlist,tarlist):
	ci = 0
	ti = 0
	while ci < len(candlist) and ti < len(tarlist):

		if candlist[ci] < tarlist[ti]:

			break
		elif candlist[ci] == tarlist[ti]:
			ci += 1
			ti += 1
		else:
			ti += 1

	if len(candlist) == ci:

		return 1
	else:
		return 0

# print the solution info
k = 2
while len(LSet[k-1]) > 1:
    #pdb.set_trace()
    print len(LSet[k-1]), str(k)+"-frequent item sets found!"
    CSet.append([])
    LSet.append([])

    GenCand(k,LSet,CSet)                    # You are supposed to complete this function
    LSet[k].sort(CompItemSet)
    CSet[k].sort(CompItemSet)
    print len(CSet[k]), str(k+1)+"-candidate item sets found!"
    #pdb.set_trace()
    for candlist in CSet[k]:
      count = 0
      for tarlist in TSet:
        count += GetFreq(candlist,tarlist)
      if count >= MinSupCount:
        LSet[k].append(candlist)
    k += 1

# write the result
outfile = file(ResultFile, 'w')
i = 1
num = 0
for fislist in LSet:
	if len(fislist) < 1:
		LSet.remove(fislist)
		continue
	num += len(fislist)
	outfile.write(str(i)+"-frequent item sets:\r\n")
	for fis in fislist:
		for itemid in fis:
			outfile.write(WordList[itemid-1])
			outfile.write('\t')
		outfile.write('\r\n')
	i += 1
outfile.close()
print num, "frequent item sets in total!"
