#适用读者
本手册适用于了解RedHadoop Workstation CRH v1.3功能的用户。
#第一章前言
关于本手册
本手册主要介绍了本公司RedHadoop Workstation CRH v1.3的基本操作方法。
#系统概述
数据伴随人类文明的成长而发展。21世纪随着互联网的发展，处理流、文本、图像、视频等非结构化海量数据系统技术有了迫切的需求。中国大量使用windows系统，Hadoop易用性、推广与windows结合有需求，Redhadoop Workstation应运而生。红象云腾公司致力于将大数据分布式技术带人更多中国企业，开展有大数据相关的基础软件平台、应用、解决方案、大数据培训等业务。基于红象云腾公司强大的底层开发实力，集成了windows系统基础上的hive数据仓库和Hadoop生态系统组件。广大的windows开发者不需要更换操作系统、不需要掌握复杂的Linux和Hadoop配置、开发，在现有操作系统和熟悉的开发环境Eclipse基础上，就能直接使用Hadoop。
概括来讲，RedHadoop Workstation CRH v1.3是红象云腾公司开发的Hadoop应用的windows版本，实现了单机版hadoop的使用，Demo演示，Eclipse开发和视频检索等功能。此外，软件还包含工具箱，能提供方便、快捷的查询功能。
Redhadoop Workstation的一大特色是在windows基础上有庞大复杂的各种应用，可以使用免移植：例如大量视频算法基于windows系统开发、应用。
而在Hadoop平台在构建视频平台上的架构优势包括以下几方面：
   -高性能:并行分析利用多服务器CPU资源；
   -高扩展:自定义UDF,将算法从摄像头端移动到服务器端；
   -高可配:通过配置VQL即可实现数据分析；
   -性价比:使用开源Hadoop平台,平台建设成本得到控制；
    Hadoop的视频计算一方面基于大规模分布式技术,海量非结构化数据，可以方便存储和计算。另一方面基于图像识别,OCR,深度学习,语音识别的技术的发展,可以使非结构化数据转化成结构化信息。此外，其范围涉及了从基于图像和声音的存储和播放到基于智能识别技术的统计、分析、挖掘等方面。

#第二章系统框架

图2-1 Redhadoop Workstation开发平台
其主要功能特点如下：
- 支持64位windows运行环境；
- 系统配置；
- 行人颜色检测；
- 车辆识别；
- Eclipse hadoop开发；
- hadoop2.4和hive0.13；
- hadoop示例演示；
- HDFS Explorer；
- 软件自动更新检查和升级；

图2-2 RedHadoop 音视频存储和分析架构

图2-3 seasbase非结构化数据库的视频检索
Seasbase数据库是能存储、处理非结构化的应用型通用数据库。
1 对文档的处理。文档是典型的非结构化数据，企业级各业务部门产生大量的工作文档，需要归类、索引、排序、使用、查找等多种需求。
2 对网页日志的处理。网页、日志是典型的半结构化数据，搜索算法是典型应用，解决web海量数据的各种需求。
3 勘探等地震波图的处理。油田、煤炭、地震地质、科学计算等2D 3D地震波图的处理，需要对P级数据做计算处理、索引、存储等应用。
4 开发视频智能检索算法的视频数据库。
解决问题的思想三步走：
1. 通过Hadoop HDFS平台分布式存储非结构化数据，。
2. 利用MapReduce 并行化加速非结构化数据处理算法，提取信息并创建索引
3. 通过SQL通用化接口，查询简化非结构化数据。
核心能力:
1. 提供PB级别大规模批量分析视频的基本能力
2. 提供丰富的后台计算视频算法，已在视频大数据处理平台应用。

图2-4 海量视频并行分析

图2-5 Hive优化流程
1.编写VideoInputFormat通过调用FFmpeg解析内容成 Key/Value (时间/图像)
com.seasbase.io.input.VideoInputFormat
2.编写 ImageSerDe 将 Key/Value 转换成字段
com.seasbase.hive.serde.ImageSerDe
3.编写 UDF 识别函数,识别出内容并返回结果
com.seasbase.hive.udf.Detect (检测)

图2-6 交通解决方案

#第三章功能描述
本软件界面主要包括菜单栏、服务工具箱、开发工具箱及查看工具箱四个模块，具体功能如下：
一、菜单栏:
主要功能如下：
1)HDFS格式化：进行HDFS格式化操作。
2)HDFS退出安全模式：刚启动的时候，NameNode会进入SafeMode状态，等待每一个DataMode报告情况。退出安全模式的时候才进行副本复制操作，以及允许数据的写入。
3)系统配置：进行系统的相关配置，包括JAVA_HOME，HADOOP_HOME，HIVE_HOME，PATH及HOSTS的配置。
4)Hadoop配置：对Hadoop相关文件的配置，包括hadoop-env.cmd，core-site.xml，hdfs-site.xml，yarn-site.xml，mapred-site.xml等文件的配置。
5)Spark配置：对Spark的master和slaves的配置。
6)查看HDFS：通过本地端口号查看HDFS的信息。
7)HDFS Explorer：启动HDFS Explorer，查询HDFS下创建的目录内容。
8)查看YARN：通过本地端口号查看YARN的信息。
9)查看SPARK：通过本地端口号查看SPARK的信息。
10)示例：提供了三个在线示例演示，包括PI，WordCount，SparkPI示例。
11)开发：提供了Eclipse集成开发和cmd命令行的调用。
12)Seasbase：进行视频检索模块的调用；
13)帮助：提供公司官网链接，进行在线咨询帮助；
14)升级：自动检查更新，进行即时版本升级；
二、服务工具箱:
 主要包含以下内容：
1)Hadoop：HDFS进程的NameNode、DataNode和Yarn进程的ResourceManager、NodeManager的启动及关闭操作。
2)Hive：Hive进程的HIVECLI及HiveServer进程的HiveServer的启动操作。
3)Spark：Spark服务进程Spark Master、 Spark Worker及Spark Shell的启动操作。
三、开发工具箱
主要包含以下内容：
1)Eclipse：提供了Eclipse集成开发。
2)命令行：cmd命令行窗口的调用。
3)颜色检测：行人颜色检测；
4)车辆检索：车辆识别（包括车牌号、车型、车身颜色）；
四、查看工具箱
主要包含以下内容：
1）编辑框：显示系统配置的相关内容。
2）查看HDFS：通过本地端口号查看YARN的信息。
3）查看YARN：通过本地端口号查看YARN的信息。
4）查看SPARK：通过本地端口号查看SPARK的信息。
#第四章操作说明
一．添加jar包和dll 
在本机RedhadoopWorkstation的安装目录下的E:\RedHadoop\RedhadoopWorkstation\CRH3\hadoop\bin路径中添加VideoReaderDll.dll。在E:\RedHadoop\RedhadoopWorkstation\CRH3\hadoop\share\hadoop\yarn路径下添加jar包。
二．启动
以管理员身份运行cmd窗口，并将目录设置为本机RedhadoopWorkstation的安装目录下（如本人RedhadoopWorkstation的安装目录为E:\RedHadoop\RedhadoopWorkstation），运行RedhadoopWorkstation.exe ；

图4-1运行RedhadoopWorkstation
在RedhadoopWorkstation的主界面上启动HDFS进程和Yarn进程；

图4-2 RedhadoopWorkstation主界面

图4-3 HDFS和Yarn启动窗口
在RedhadoopWorkstation工程中启动程序，在菜单栏中选择配置-HDFS退出安全模式，保证HDFS未在安全模式下；
如需要，可在RedhadoopWorkstation的主界面上启动Hive进程进行相关操作；

图4-4 启动Hive
同样，如需要，可在RedhadoopWorkstation的主界面上启动Spark进程进行相关操作；

图4-5 启动Spark Master
图4-6 启动Spark Worker

图4-7启动 Spark Shell
#三．操作
1）颜色检测模块
1. 在菜单栏中点击Seasbase-视频检索，或是在开发工具箱中点击颜色检测按钮，进入颜色检索画面；

图4-8颜色检测界面
2. 点击Load Videos按钮，进入加载视频窗口，输入表名，选择视频文件路径，分片尺寸，点击Load按钮，加载视频，并为其创建表和索引,，此过程需要一点时间等待；在HDFS Exploer中的HDFS Connections-localhost-user-hive-warehouse下可以查看创建的yst01291012表，里面包含索引，此目录下的Video+表名（Videoyst01291012)文件夹中为加载的视频文件；

图4-9 加载视频窗口

图4-10 索引文件

图4-11视频文件
3. 点击Load Tables按钮，加载创建的表，并显示在TableList下的树型列表中（见图1-8左侧树型列表）；
4. 选中指定的表名，点击LoadImage按钮，加载一张图片，在感兴趣区域点击对角线两点，框出矩形区域，画出其颜色直方图，并给出重要的颜色信息，在编辑框中设置select语句，点击“Excute”按钮，执行mapreduce计算检索，并将结果显示在Image列表中；

图4-12 检索结果

图4-13 颜色直方图
2）车辆识别模块
1. 在菜单栏中点击Seasbase-车辆识别，或是在开发工具箱中点击车辆识别按钮，首先要进行模块的初始化，这需要一点时间等待，然后会进入车辆识别画面；

图4-14 模块初始化

	图4-15 车辆识别界面
2. 点击Load Videos按钮，进入加载视频窗口，输入表名，选择视频文件路径，分片尺寸，点击Load按钮，加载视频，并为其创建表和索引,，此过程需要一点时间等待；在HDFS Exploer中的HDFS Connections-localhost-user-hive-warehouse下可以查看创建的t01161706表，里面包含索引，此目录下的Video+表名（Videot01161706)文件夹中为加载的视频文件；


图4-16 加载视频
	
图4-17 索引文件

图4-18 视频文件
3. 点击Load Tables按钮，加载创建的表，并显示在TableList下的树型列表中（见图1-15左侧树型列表）；
4. 选中指定的表名，点击LoadImage按钮，加载一张车辆的图片，点击Indetify按钮，将执行车辆识别操作，识别车辆的车牌号，车型和车身颜色，并将信息添加到vehicle Message 列表中，在编辑框里自动生成select语句，点击“Excute”按钮，执行mapreduce计算检索，并将结果显示在Image 列表中；

图4-19 车辆检索结果（黑色车）
3）Hadoop示例演示
1. 在HDFS进程启动环境下，可以点击菜单-示例-Pi或WordCount来查看Demo演示。

图4-21  Pi示例运行结果
2. 在WordCount运行时，需根据提示先选择一个或多个后缀名为.txt或.log的文件，例如下图的test.txt文本文件。

图4-22 文本文件
  运行结果图如下：

图4-23 WordCount 统计结果
4）工具箱
1.启动了HDFS进程后，可以利用菜单-入口-查看HDFS或是工具箱中的查看HDFS来即时查看相关信息；

图4-24 HDFS信息
2.同样，在启动了Yarn进程后，也可以利用菜单-入口-查看Yarn或是工具箱中的查看Yarn来即时查看相关信息；

图4-25 YARN信息
3.同样，在启动了Spark进程后，也可以利用菜单-入口-查看Spark或是工具箱中的查看Spark来即时查看相关信息；

图4-26 SPARK信息
4. 启动HDFS Exploer，可以查询HDFS下创建的目录内容；
5. 可以进行Eclipse hadoop开发；
5）升级
    软件启动时会自动检测版本信息，能快速检测出是否有可用更新的软件，提醒用户进行软件更新，一键升级，体验新功能。



图4-27 软件自动检查更新
