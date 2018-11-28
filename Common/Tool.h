#include <string>
#include <ctime>
using namespace std;

namespace Tool {
	static vector<string> split(const string& str, const string& delim) {
		vector<string> res;
		if ("" == str) return  res;

		string strs = str + delim; //*****��չ�ַ����Է���������һ���ָ������ַ���
		size_t pos;
		size_t size = strs.size();

		for (int i = 0; i < size; ++i) {
			pos = strs.find(delim, i); //posΪ�ָ�����һ�γ��ֵ�λ�ã���i��pos֮ǰ���ַ����Ƿָ��������ַ���
			if (pos < size) { //������ҵ������û�в��ҵ��ָ�����posΪstring::npos
				string s = strs.substr(i, pos - i);//*****��i��ʼ����Ϊpos-i�����ַ���
				if (s.size())
					res.push_back(s);
				i = pos + delim.size() - 1;
			}

		}
		return res;
	}

	template<typename T>
	static inline void time_it(T(*pf)())
	{
		time_t start = clock();
		pf();
		time_t end = clock();
		cout << "Time consumed: " << (double)(end - start) / CLOCKS_PER_SEC << "s\n";
	}
}