#coding=utf-8
import os
from os.path import join
import jieba
import codecs
import re

source = ".\\sourse"
dest = ".\\result"

def removeDir(dirPath):
    if not os.path.isdir(dirPath):
        return
    files = os.listdir(dirPath)
    try:
        for file in files:
            filePath = os.path.join(dirPath,file)
            if os.path.isfile(filePath):
                os.remove(filePath)
            elif os.path.isdir(filePath):
                 removeDir(filePath)
        os.rmdir(dirPath)
    except:
        pass

def remove_nouse_char(s):
    new_s = str(s).replace('？', '').replace('✧','').replace('-','')\
        .replace('=','').replace('“','').replace('”','').replace('。','')\
        .replace('，','').replace('【','').replace('】','').replace('；','')\
        .replace('《','').replace('》','').replace('—','').replace('.','')\
        .replace('！','').replace('~','').replace('（','').replace('）','')\
        .replace(':','').replace('…','').replace('：','').replace('＃','')\
        .replace('#','').replace('/','').replace('「','').replace('」','')\
        .replace('+','').replace('*','').replace('%','').replace('、','')\
        .replace('〜','').replace('!','').replace('"','').replace('$','')\
        .replace('&','').replace('\'','').replace('(','').replace(')','')\
        .replace(',','').replace(';','').replace('<','').replace('>','')\
        .replace('?','').replace('@','')
    return new_s

index = codecs.open(dest + '\\' + 'index.txt','w')
count_index = 0
#遍历目录下文件
for dirname in os.listdir( source ):
        print(dirname)
        #二级目录名
        filepath = os.path.join(source, dirname)
        print(filepath)
        #不是文件夹，跳过  
        if not (os.path.isdir(filepath)):
            continue
        destpath = dest + '\\' + dirname
        try:
            os.mkdir(destpath)
        except:
            removeDir(destpath)
            os.mkdir(destpath)
        for OneFileName in os.listdir( filepath ) :
            #不是文件，跳过
            if OneFileName.find( '.txt' ) == -1 :
                continue
            #确认为txt文件
            print("从 " + OneFileName + " 读入")
            f = codecs.open(filepath + '\\' + OneFileName, 'r', 'utf-8')
            NewFileName = OneFileName.replace('.txt', '.new').replace(' ','')
            try:
                index.write(str(count_index) + ' ' + dirname + '\\' + NewFileName + '\n\r')
            except:
                continue
            count_index += 1
            Newf = codecs.open(destpath + '\\' + NewFileName, 'w+', 'utf-8')
            line = 1
            while line:
                #逐行读取文件
                line = f.readline()
                s = str(line).replace(' ','')
                #文件内容匹配
                #res = re.search(r'url = "(.*?)" name = "(.*?)"',line)
                res = re.search(r'href="(.*?)"title="(.*?)"author="(.*?)"content="(.*?)"',s)
                #如果该行未找到指定的匹配文本，跳过这一行
                if not res:
                    continue
                #使用jieba分词
                seg_list = jieba.cut_for_search(remove_nouse_char(res.group(2))\
                                             + remove_nouse_char(res.group(4)))
            
                #使用/给中文字符串分词
                NewContent = " ".join(seg_list)
                #输出到.new文件中
                Newf.write(NewContent)
            
            print("输出到：" + NewFileName)
            Newf.close()
            f.close()

index.close()
print('SUCCESS')
