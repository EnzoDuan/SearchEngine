# Search Engine Demo

##问题描述

实现一个中文搜索引擎的原型系统（demo 版本）。检索（即搜索）的范围可以是某个网站的网页（比如南京大学的内部网页），也可以针对某个行业做垂直搜索（比如学术论文检索），或者针对社交媒体（比如微博）进行检索。

**爬虫：**我们实现的是进行知乎网站的精华搜索。因此需要获取到知乎网站的精华部分的内容，这就需要利用爬虫将精华部分的数据爬取到本地并进行整理便于后期检索。

**中文分词：**输入一串中文字符串，根据中文语义环境将其分为多个中文词组，以便于搜索引擎将爬下的中文文档进行分析处理。

**倒排索引**：提供index.txt(文件编号-文件名索引文件)，以及中文分词分好后的所有数据文件(.new格式)，对其按照字典序建立倒排表，并生成输出。

**B+树**：根据倒排索引，构建B+树索引提高读取效率。

**布尔检索**：根据正常检索格式，检索出所有的条目，然后将条目按照AND以及OR进行排序。

**PageRank**：基于文档之间的有向链接图，实现 PageRank 排序算法，用来对布尔检索返回的结果(文档)进行排序。另一种对布尔检索返回的 结果进行排序的方法是基于每个文档被其他文档链接的次数(即有向图中的入度)，入度大的排在前面。实现基于入度的排序算法，并跟 PageRank 排序算法得到的排序结果进行对比。在布尔检索返回的 结果中，同时显示文档、文档对应的 PageRank 值和入度。

**布尔检索：**根据分词结果在文件中搜索到所有条目，根据条目的包含分词结果的与或关系进行更替排序。

**界面设计：**利用Qt进行UI设计。将用户输入的字符串分好词后写入in.txt，供布尔查找，查询器寻找到指定的条目后返回到out.txt文件中，ui此时将结果返回出来。

##数据结构设计

**爬虫**：因为[要]()对最后的数据进行检索，因此在进行爬虫爬取的过程之中就需要对数据进行处理以方便后期[的检索]()。在进行爬取过程中，知乎网站比较特殊需要登录才能进行爬取，因此选择python的自动化库进行自动登录然后根据find_text找到下一页点击进行下一页的爬取。考虑到效率问题，利用队列和分线程实现了分布式爬取，提高了速度。然后将最后的数据整理成如下形式：

```html
href ="http://www.zhihu.com/question/24342598/answer/52856936" 
title ="**在南京大学就读是怎样的体验？**" 
author ="{'author': '**戴逸**', 'href': 'http://www.zhihu.com/people/dai-yi-2930','intro': '\n**游泳教练**/**大魔导师**\n'}" 
content ="**谈学校的虚名多无趣啊，也根本毫无意义。既然是说体验，讲点私人化的东西可好？** 11**级的，马上要滚蛋了，坐在箱子上写写这些没人看的玩意儿。** 0. **周二毕业典礼把几千人哄挤在体育馆里公放视频看，几乎每个系的视频都稚嫩、严重小清新、散发着荷尔蒙失调的磅…**"
```

这样方便最后的检索和匹配，利用正则表达式就很方便进行最后结果的匹配。所有数据在一行，第一位为行数，第二位为入度，第三位为Pagerank值*100,000（方便不损失精度存储），接下来为链接、该问题的题目、答主的个人信息、回答的内容。这样会将所有的内容进行匹配然后按照我们最后的设计显示出来。

**倒排索引**：

考虑到倒排表的实现是由“文件名——中文词组数组”的对应关系到“中文词组——文件名数组”的对应关系的转化过程，我们原来准备采用结构体{文件名，中文词组}并且遍历的方式来进行倒排索引，但是考虑到这种方式的时间复杂度不小，效率较低，于是我们又决定采用性能更好的，自带红黑树的C++ stl容器map来实现这一层对应关系，数组的实现采用vector容器（自己实现）。

**B+树：**

生成B+树的时候，数据总共是35W+条，分词最后的结果为52W+，因此需要建立724阶的B+树进行最后的检索，两层。为了减少文件的读取，在顶层的索引文件中写入每一条索引对应文件的第一条内容和最后一条内容，这样就可以直接在顶层检索中定位好是哪个二层文件。[如下为顶层条目样例]()：

```
*107 first = {word = "**三金**" Source =" 594|1 1340|1"} last = {word = "**上岗在**" Source = " 432|1"}*
*// 107**为第**107**号二层文件，**word**为分词检索结果，**Source**为出现的根文件编号***
如下为二层条目样例：
*word = "**三金**" Source = " 594|1 1340|1"                          //**二层文件第一条***
*word = "**上岗在**" Source = " 432|1"                                    // **二层文件最后一条***
```

PageRank：用链表存储，构建邻接表，结点结构如下：

```c++
struct URL{
    Fractionpagerank;//分数类
    intdegree;//度
    intNo;//编号
    URL*next;//链向下一个结点
    URL*link;//链向邻接结点
};
```

**布尔检索**：

在检索模块中，先根据B+树找到对应的二层条目，然后正则匹配出对应的source文件。进入对应的suorce文件进行检索。这就是整个B树搜索的思路和数据结构。

层次检索结构体如下：

```c++
struct Result {
	double	PageRank;
    int indeg;
    string content;
    string file;
    string word;
    Result * next;
};
```

```c++
struct SearchNodeTop {
	string first_word;
    string first_PageRank;
    string last_word;
    string last_PageRank;
};
```

```c++
struct SearchNodeBT {
	string word;
    string PageRank;
    SearchNodeBT *next;
};
```

```c++
structSearchNode {
	string word;
    int file;
    SearchNode *next;
};
```

```C++
struct Index {
	int num;
    string file;
};
```

##算法设计

**爬虫：**首先自动化登录，手动输入验证码，每爬取一个主题创建一个主题的文件夹，然后点击子话题进入精华部分，通过翻页进行爬取，每一个子话题创建一个文本文件，输出格式为上述数据结构中讲述的格式，这样就能够完成知乎话题精华的爬取，并且方便了检索。

**中文分词：**们采用的方案是使用github上的开源分词软件“结巴jieba”进行中文分词，由于jieba是基于python语言开发的，所以还需要相应的python环境支持。

**倒排索引**有了数据结构，算法的实现很简单，根据index.txt索引文件一个个打开编号为id的数据文件，用map<string,vector<int>>将这些文件中的词组存下，文件名数组push进此时文件编号id，等到所有的文件读入后，使用map的迭代器iterator遍历输出整个map结构（自成字典序）。

**B+树算法：**表中数据按主键顺序存放。而聚集索引就是按每张表的主键构造一颗B+树。并且叶节点存放整张表的行记录数据。每张表只能有一个聚集索引（一个主键）。聚集索引的另一个好处是它对于主键的排序查找和范围的速度非常快。叶节点的数据就是我们要找的数据。（代码见代码部分）

**PageRank**：基于爬虫程序，我们得到一份根话题链接的索引文件index.txt（主要存储该链接网页的爬取内容的txt文本文件）以及对应于索引文件的所有爬取的内容；

​           1、利用索引文件，遍历所有源文件，正则匹配出其中的网址

​           2、建立基于所有链接之间关系图的邻接表，并在建立时直接统计链接的度

​           3、利用邻接表，模拟邻接矩阵，根据pagerank算法进行计算

​           但是，在本次试验中，我们总共统计出了近50万条链接，本机电脑运算速度对O(n ^2)的运算也需要极长的时间，所以直接运算并没有效果。由于数据的特殊，发现在子话题url之间链接相比于整个数据量可以近似忽略不计，所以，在建立邻接表时，将根话题与子话题url分开，可以对邻接表得到的的邻接矩阵先分块再运算，把子话题url之间链接近似看作0，这样，主要的链接关系就是根话题与子话题之间与根话题与根话题之间的链接关系，由于子话题链接占绝大多数，对其近似考虑后省去大部分运算，迭代计算最终得到pagerank。

**布尔检索**：检索部分算法为按照折半查找法先从B+顶层进行查找，找到二层索引之后再次利用折半查找进行[查找]()，找到我们对应的索引，正则匹配出出现在的文件号，在index中折半查找找到文件号以及对应的source文件的文件夹以及文件名。

然后为AND和OR部分实现，实现集合类(bitSet)，对集合进行基本的与或运算。将搜索到的包含关键词的链表按照关键词分链，对每一条链构建不同的位向量集合并使链中包含的结点所代表的位置为1，并对整体进行“与”和“或”运算得到新的集合，根据新的位向量集合找出对应位为1的结点，构造出一条新链，最后实现按照pagerank排序，排序采用直接插入排序。

由于每次输入的关键词不确定，通过jieba分词得到的结果数目不同，所以分链过程采用递归方式进行，每次将链分为两部分，链A包含同一关键词，不包含该关键词的结点放在链B，对链A进行集合运算后，再递归调用链B，直到链B为空为止，递归最终得到的结果是一组位向量存储的集合，为了减少代码，该递归函数添加参数type，区别与或者非运算。