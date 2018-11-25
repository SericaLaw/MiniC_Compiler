class Helper {
private:
    Helper();
    ~Helper();
public:
    static struct tool {
        enum color {
            RED,
            BLUE,
            GREEN
        };
        enum sex {
            MALE,
            FEMALE
        };
    } tool;
    static bool greater_or_equal_than(int a, int b) {
        if((a >= b) && ((a > b) || (a == b))) 
            return true;
        return false;
    }
};



int main() { 
    typedef double myDouble;
    myDouble myNum = 10.5;
    char c = 'a';
    const int hisNum = (int) c;
    int herNum = myNum - hisNum;
    bool flag = true;
    do {
        myNum %= 22;
    } while (!flag);
    for(int i = 0; i < 100; i++) {
        if(herNum == myNum) {
            break;
        }
        else if(Helper::greater_or_equal_than(herNum, myNum)) {
            herNum--;
            continue;
        }
        else {
            herNum++;
            continue;
        }
        myNum = (myNum != hisNum ? ++myNum : --myNum);
    }
    switch(herNum) {
        case 10:
            herNum *= myNum;
            break;
        case 20:
            herNum = Helper::tool.RED;
        default:
            herNum -= myNum;
    }
    return 0;
}
