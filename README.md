# AQL子集编译器

## 代码运行
```bash
# 在src目录下执行

# 运行命令
make

# 清理命令
make clean
```

## 代码结构
```bash
.
├── dataset
│   ├── art
│   │   ├── Art.input
│   │   └── Art.output
│   ├── Art.aql
│   ├── dataset_readme.txt
│   ├── perloc
│   │   ├── PerLoc.input
│   │   ├── PerLoc.output
│   │   └── PerLoc.standard.output
│   ├── PerLoc.aql
│   ├── revenue
│   │   ├── Revenue.input
│   │   ├── Revenue.output
│   │   └── Revenue.standard.output
│   └── Revenue.aql
├── README.md
└── src
    ├── code_token.cpp
    ├── code_token.h
    ├── common.cpp
    ├── common.h
    ├── doc_token.cpp
    ├── doc_token.h
    ├── lexer.cpp
    ├── lexer.h
    ├── main.cpp
    ├── makefile
    ├── parser.cpp
    ├── parser.h
    ├── regex.cpp
    ├── tokenizer.cpp
    └── tokenizer.h
```


## 代码说明

### 基础通用函数域
> `common.h`和`common.cpp`

这里我定义了一个`Common`的域，该域包含了其他代码中常用到的一些通用函数：

* `error`: 错误输出，并终止
* `string_format`: 字符串格式化
* `is_char`: 判断字符串是否全部为字符（a-zA-Z0-9）
* `is_num`: 判断字符串是否全部为数字
* `is_symbol`: 判断字符串是否全部为特殊符号
* `string_to_int`: 将`string`类型转换为`int`类型
* `get_file_name`: 从文件路径中获取文件名

### AQL代码token结构类
> `code_token.h`和`code_token.cpp`

该类定义了AQL代码的`token`结构：

* `value`: `CodeToken`的值
* `type`: `CodeToken`的类型
* `line_number`: `CodeToken`所在的行数

另外方便测试也有打印的`print`函数

### 词法分析器
> `lexer.h`和`lexer.cpp`

该类定义了AQL的词法分析器，主要功能就是从文件中读取AQL源代码，并将其转换成一个个的`CodeToken`，存放在一个`vector`，方便以后的语法分析：

* `get_code_tokens`: 把源代码文件内容转换为`CodeToken`，并用一个`vector`数组存储

### 文章token结构类
> `doc_token.h`和`doc_token.cpp`

该类定义了文章的`token`的结构：

* `value`: `DocToken`的值
* `start_pos`: `DocToken`的起始位置
* `end_pos`: `DocToken`的终止位置

同时也有方便打印的`print`函数和`to_string`函数

### 文章`token`分析器
> `tokenizer.h`和`tokenizer.cpp`

该类定义了文章的`token`分析器，主要功能为将文章划分为一个个的`token`，存放在一个`vector`，方便以后的语法分析：

* `get_content`: 得到一个包含文章内容的字符串
* `get_doc_tokens`: 把文章内容转换为`DocToken`，并用一个`vector`数组存储

### 语法分析器
> `parser.h`和`parser.cpp`

该类定义语法分析器，也是这个编译器的核心，之前的所有内容也都是为此做准备，主要内容就是解释词法分析出来的`CodeToken`，构建语法树，解析其内容，根据内容对文章进行处理，这一部分的思路其实也不复杂，主要还是跟着生成式走，通过函数递归调用从顶层函数调用至底层，最后达到递归下降的效果，处理生成式的函数及其对应的生成式如下：

* `aql_stmt`: aql_stmt → create_stmt ; | output_stmt ;
* `create_stmt`: create_stmt → **create view ID as** view_stmt
* `output_stmt`: output_stmt → **output view ID as ID**
* `view_stmt`: view_stmt → select_stmt | extract_stmt
* `select_stmt`: select_stmt → **select** select_list **from** from_list
* `select_list`: select_list → select_item | select_list , select_item
* `select_item`: select_item → **ID . ID as ID**
* `extract_stmt`: extract_stmt → **extract** extract_spec **from** from_list
* `from_list`: from_list → **ID ID** | from_list **, ID ID**
* `extract_spec`: extract_spec → regex_spec | pattern_spec
* `regex_spec`: regex_spec → **regex REG on ID.ID** name_spec
* `pattern_spec`: pattern_spec → **pattern** pattern_expr name_spec
* `pattern_expr`: pattern_expr → pattern_pkg | pattern_expr pattern_pkg
* `pattern_pkg`: pattern_pkg → atom | atom_by_range | pattern_group
* `name_spec`: name_spec → **as ID** | **return** group_spec
* `atom`: atom → ** < ID.ID > ** | ** < Token > ** | **REG**
* `atom_by_range`: atom_by_range → atom**{ NUM , NUM }**

上面的生成式可能和给出的略有不同，我是为了便于处理做了一定的修改，但实际效果应该是差不多的。另外最后还有几个辅助函数，没有在生成式中表现出来，但是在其中也是起到了重要作用：

* `link`: 将两个`DocToken`数组进行拼接，在`pattern`的处理中使用
* `is_connect`: 判断两个`DocToken`是否相邻
* `find_all_to_token`: 通过正则表达式找到所有的结果，并以动态数组的格式返回
* `void view_output`: 格式化打印结果