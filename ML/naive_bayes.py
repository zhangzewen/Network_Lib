#encoding=utf-8
#########################################
##利用朴素贝叶斯概率，进行新闻文章的分类。
##linux 下运行\如果是windows，需要改一下listdir的路径写法
## 测试方式1（）：
## data目录下 存放了1264个训练文档
## data2目录下  存放了 50个 测试文档
## 测试方法2 ：
## data目录下存放全部3066个文档，随机选择20%作为测试集、80%为训练集
#########################################

import os
import re
import pickle
from numpy import *
dirs = os.listdir("./data")
#print dirs
#新闻分类及p（yi）
vol=[]
regex=re.compile('\d+(\w+)')
dirset=[regex.match(i).group(1) for i in dirs]

type=list(set(dirset))
print type

###计算先验概率,全部文档
def create_proyi(dirs):
	count1=0;count2=0;count3=0
	pattern=re.compile(r'\d+(sports)')
	py=[]
	for i in dirs:
		#print i
		m=pattern.match(i)
		if m:	
			#print m.group(1)
			count1=count1+1
		if (re.match(r'\d+(auto)',i)):
			#print (re.match(r'\d+(business)',i)).group(1)
			count2=count2+1
		if (re.match(r'\d+(business)',i)):
			#print (re.match(r'\d+(sports)',i)).group(1)
			count3=count3+1
	print count1,count1/float(len(dirset))
	print count2,count2/float(len(dirset))
	print count3,count3/float(len(dirset))
	py.append([count1/float(len(dirset)),count2/float(len(dirset)),count3/float(len(dirset))])
	return py
	


##创建词库，
#记录每个词在3066篇文章中出现的文章篇数（TF-INF）。词库只输出出现50篇文章以下的
def createVocal(alldoc):
	fcount=0
	hs_w={}
	hs_word={}
	hs_file={}
	hs_file_i={}
	for dir in alldoc:
		
		fcount=fcount+1
		file=open("data/"+dir,'r')
		for str in [line.split() for line in file]:
				
			for i in str:
				
				if (not hs_file_i.has_key(dir+i)):#该文档内的某一词出现过，该词则不++了
					hs_word[i]=hs_word.get(i,0)+1 			
				hs_file_i[dir+i]=1									
				
		hs_file[dir]=1
		
	count=0
	fw=open('type.txt','w')#存储下纳入词库的，方便查看到底是哪些词
	dr=open('drop.txt','w') #扔掉的，也就是在多个文章出现的词。
	vocabSet=[]
	for key in hs_word:
			#print sorted(hs_word.items(),key=lambda d:d[1])
			#o=key+str(hs_word[key])
			f1=key
			f2=bytes(hs_word[key])
			out=f1+f2
			if hs_word[key] <50:
				#print key,hs_word[key]
			
				fw.write(out);fw.write('\n')
				count=count+1
				vocabSet.append(f1)
			else:
				#print key,hs_word[key]
				dr.write(out+'\n')
				
	fw.close()
	dr.close()
	return vocabSet
	print "hs_word count:",len(hs_word),"After:",count
	


#输出某文档向量，如果是词集中的词，则将词集向量中的0改为1。最后调整了算法，出现多次则++
def setofWords2vec(vocalblist,input):
	returnVec=[0]*len(vocalblist)
	for word in input:
		if word in vocalblist:
					 
			returnVec[vocalblist.index(word)] +=1
	return returnVec
#创建全部文档的向量

def crtallVec(alldoc):
	
	allmatrix=[]
	trainSet=alldoc;testSet=[]
	
	for i in range(int(len(alldoc)*0.2)):
		randIndex=int(random.uniform(0,len(trainSet)))
		#print 'randIndex',randIndex
		
		testSet.append(alldoc[randIndex])
		del(trainSet[randIndex])
	
		
	#trainMat=[];trainClasses=[]
	#testSet.append(alldoc[randIndex])
		
	labels=[]
	count=0
	#for dir in alldoc:
	for dir in trainSet:
		vocinput=[]       

		###################label#################################
		
		#dir=[regex.match(i).group(1) for i in dirs]
		label=regex.match(dir).group(1)
		labels.append(label)
		#print 'label,dir',label,dir,labels
		####################################################
		
		file=open("data/"+dir,'r')
		for line in file:
			line1=line.split()
			for line2 in line1:
				#if line2 not in vocinput:
				vocinput.append(line2)
        	vec=setofWords2vec(vocal,vocinput)       		
		#print vec
		allmatrix.append(vec)
		#####################################################
		count=count+1
		print 'Document count:',count,
	#print allmatrix
	
	#flab.write(str(labels))
	#fvet.write(str(allmatrix))

	# 序列化对象，存到磁盘。方便多次测试时，无需重新创建向量
	with open('allVec','wb') as fallvet:
		pickle.dump(allmatrix,fallvet)
	with open('allLable','wb') as falllable:
                pickle.dump(labels,falllable)


	print 'len(allmatrix):',len(allmatrix),'len(label):',len(labels)
	return allmatrix,labels,testSet
	#return allmatrix,labels
##############################################
##训练文档，输出每个vocabulary|分类 的概率向量
def trainNB(trainM,trainLabel):
	numDocs=len(trainM)
	numWords=len(trainM[0])
	print 'numDocs:',numDocs,'numWords:',numWords
	#psportsNum = zeros(numWords);pautoNum=zeros(numWords);pbusinessNum=zeros(numWords)
	psportsNum = ones(numWords);pautoNum=ones(numWords);pbusinessNum=ones(numWords)
	#psportsDenom=0.0;pautoDenom=0.0;pbusinessDenom=0.0
	psportsDenom=2.0;pautoDenom=2.0;pbusinessDenom=2.0
	for i in range(numDocs):
		if trainLabel[i] == 'sports':
			psportsNum+=trainM[i]
			#sports总词ji数
			psportsDenom += sum(trainM[i])
			#print psportsDenom
		elif trainLabel[i]=='auto':
			pautoNum+=trainM[i]
			pautoDenom += sum(trainM[i]) #auto总词ji数
		else:
			pbusinessNum+=trainM[i]
			pbusinessDenom += sum(trainM[i])
		#print trainL[i],trainM[i]
	fvet=open('tt.txt','w')
	
	fvet.write(str(psportsNum))
	#psportsVec=psportsNum/psportsDenom
	psportsVec=log(psportsNum/psportsDenom);pautoVec=log(pautoNum/pautoDenom);pbusinessVec=log(pbusinessNum/pbusinessDenom)
	#print 'psportsNum/psportsDenom:',psportsNum/psportsDenom,'log:',psportsVec,sum(psportsNum/psportsDenom)
	print 'len(psportsNum):',len(psportsNum),sum(psportsNum),psportsDenom
	print 'psportsDenom:',psportsDenom,pautoDenom,pbusinessDenom
	return psportsVec,pautoVec,pbusinessVec
##############分类方法######################################	
def classfyNB(vec2class,psportsVec,pautoVec,pbusinessVec,py):
	pmax=[]
	pyi=py
	#input=[str for str in [line.split() for line in first]]
	#后分类概率
	psports=sum(vec2class*psportsVec)+log(pyi[0][0])
	pauto=sum(vec2class*pautoVec)+log(pyi[0][1])
	pbusiness=sum(vec2class*pbusinessVec)+log(pyi[0][2])
	#print 'psports:',psports,'pauto:',pauto,'pbusiness',pbusiness
	#print 'log(pyi[0][0])',log(pyi[0][0]),pyi[0][0],'pyi[0][1]',pyi[0][1]
	pmax.append([psports,pauto,pbusiness])
	pmax.append(['sports','auto','business'])
	#print 'max(pmax[0]):',max(pmax[0])
	#print 'Result:',pmax[1][int(pmax[0].index(max(pmax[0])))]
	return pmax[1][int(pmax[0].index(max(pmax[0])))]
###############################################
###################分类验证  ，应该采用上面注释掉的方式验证。随机选择训练集和测试集#########
def newsClass(testSet):
	
	bingo=0
	wrong=0
	total_t=0
	#testSet = os.listdir("./data2/")
	for dir in testSet:
		vocinput=[]
		#file=open("data2/"+dir,'r')
                file=open("data/"+dir,'r')
		####################################################
                for line in file:
                        line1=line.split()
                        for line2 in line1:
				#if line2 not in vocinput:
				vocinput.append(line2)
                vec=setofWords2vec(vocal,vocinput)
		
		fname=re.match(r'\d+(\w+)',dir).group(1)
		
		print 'fname:',fname
		classResult=classfyNB(vec,psportsVecIn,pautoVecIn,pbusinessVecIn,pyi)
		print 'classResult',classResult
		if classResult==fname:
			bingo=bingo+1
			total_t+=1
			print 'bingocount:',bingo
		else:
			wrong=wrong+1
			total_t+=1
		print 'Total_test:',total_t
	print 'bingo:',bingo,'len(testSet):',len(testSet),"pBingo:",bingo/float(len(testSet))
	print 'wrong:',wrong,'pWrong',wrong/float(len(testSet))
######################################################################
pyi=create_proyi(dirs)   #计算先验概率
print 'pyi:',pyi,pyi[0][0],pyi[0][1]
vocal=createVocal(dirs)  #创建词库
allfiles = os.listdir("./data")   #读取训练文件目录

allmatrix,labels,testSet=crtallVec(allfiles)

###########多次交叉验证时，从硬盘读取转化好的文档向量和标签，不用每次再创建向量了################
'''
readvec=open('671Vec','r')
readlab=open('671Lab','r')
allmatrix=pickle.load(readvec)
labels=pickle.load(readlab)
'''
#print 'pickle::len(allvec)','pickle::len(alllab)',allvec


psportsVecIn,pautoVecIn,pbusinessVecIn=trainNB(allmatrix,labels)
print 'yanzheng:',psportsVecIn,pautoVecIn,pbusinessVecIn


newsClass(testSet)  #分类验证,  读取./data2/  测试文件目录


######################################################
#################单文件验证##########################

testfiles = os.listdir("./data2") 
for files in testfiles:
	vocinput=[]
	first=open("data2/"+files,'r')
	for line in first:
			line1=line.split()
			for line2 in line1:
				#if line2 not in vocinput:
					vocinput.append(line2)	
		
	vec=setofWords2vec(vocal,vocinput)
	#############################################
	result=classfyNB(vec,psportsVecIn,pautoVecIn,pbusinessVecIn,pyi)
	print 'The result is:',result,first
'''
