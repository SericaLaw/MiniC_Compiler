// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO: 在此处引用程序需要的其他头文件


#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include <string>
using std::string;
using std::pair;
using std::find;

#include <stack>
using std::stack;

#include <vector>
using std::vector;

#include <queue>
using std::queue;

#include<set>
using std::set;

#include<map>
using std::map;

#include <cassert>
#include <algorithm>
using namespace std;

#include "Regex.h"
#include "FANode.h"
#include "NFA.h"
#include "Optimizer.h"
#include "TestRegex.h"
#include "TestNFA.h"
#include "TestOptimizer.h"