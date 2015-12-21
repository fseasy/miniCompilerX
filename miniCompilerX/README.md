#miniCompiler

miniCompilerX项目完成词法、语法、语义分析。

输入：*.x  符合文法的源文件

输出：*.ix 三地址码

###文件夹及文件定义说明：


bin 下是已编译好的程序和需要读取的文件。

CB_project 是CodeBlocks项目文件

src 是整个工程的源代码。


假设定义的语言为x语言

源代码文件后缀为.x

生成的三地址码文件后缀为.ix

词法分析输出*.pro(预处理输出) ,*.out(词法输出)

###源代码文件构成：

	/src : 
			keywords.txt x语言的关键字表
			noneterminallist.txt 文法非终结符
			terminallist.txt 文法终结符
			production.txt 产生式
			test1.x 测试源文件1(代码中默认使用1)
			test2.x 测试源文件2
			main.c 项目主函数
	/src/lex  :
			constant.h 定义的常量
			dbuffer.h 双缓冲定义
			hash.h 散列函数
			keyword.h 处理关键字
			preprocess.h 预处理x语言源文件(去除多余空格回车和注释)
			stack.h 栈
			token_code.h 关键字宏(实际中没有用到)
			lexical.c 词法处理主程序
	/src/semantic :
			genfile.c 用于根据产生式自动生成语义动作框架
			semantic_act.c 定义语义动作
			semantci_analysis.h 定义语义分析用到的数据结构，函数声明
			semantic_analysis.c 函数实现
	/src/syntax :
			analysis_table.h 定义生成LR1分析表所需要的数据结构
			analysis_table.c 用于生成LR1分析表的函数
			syntax.c 语法分析
			