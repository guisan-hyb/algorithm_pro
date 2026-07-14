#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

using namespace std;


//哈希函数的扩展作用 （核心）
//利用哈希函数的均匀性，可以把样本进行均匀分组


//工程上常用的哈希函数：
//SHA3-512, SHA-1, SHA-384,
//SHA3-384, SHA-224, SHA-512/256
//SHA-256, MD2, SHA-512/224, SHA3-256,
//SHA-512, SHA3-224, MD5

//展示一下 用法 & 均匀性

#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <cstdint>
#include <cstring>
#include <algorithm>

// ==================== MD5 Implementation ====================

class MD5 {
private:
    uint32_t state[4];
    uint64_t count;
    uint8_t buffer[64];
    uint8_t digest[16];
    bool finalized;

    static uint32_t F(uint32_t x, uint32_t y, uint32_t z) { return (x & y) | (~x & z); }
    static uint32_t G(uint32_t x, uint32_t y, uint32_t z) { return (x & z) | (y & ~z); }
    static uint32_t H(uint32_t x, uint32_t y, uint32_t z) { return x ^ y ^ z; }
    static uint32_t I(uint32_t x, uint32_t y, uint32_t z) { return y ^ (x | ~z); }
    static uint32_t rotate_left(uint32_t x, int n) { return (x << n) | (x >> (32 - n)); }

#define FF(a,b,c,d,x,s,ac) { (a) = rotate_left((a) + F((b),(c),(d)) + (x) + (uint32_t)(ac), (s)) + (b); }
#define GG(a,b,c,d,x,s,ac) { (a) = rotate_left((a) + G((b),(c),(d)) + (x) + (uint32_t)(ac), (s)) + (b); }
#define HH(a,b,c,d,x,s,ac) { (a) = rotate_left((a) + H((b),(c),(d)) + (x) + (uint32_t)(ac), (s)) + (b); }
#define II(a,b,c,d,x,s,ac) { (a) = rotate_left((a) + I((b),(c),(d)) + (x) + (uint32_t)(ac), (s)) + (b); }

    void transform(const uint8_t block[64]) {
        uint32_t a = state[0], b = state[1], c = state[2], d = state[3];
        uint32_t x[16];
        for (int i = 0, j = 0; j < 64; i++, j += 4)
            x[i] = ((uint32_t)block[j]) | ((uint32_t)block[j + 1] << 8) |
            ((uint32_t)block[j + 2] << 16) | ((uint32_t)block[j + 3] << 24);

        FF(a, b, c, d, x[0], 7, 0xd76aa478); FF(d, a, b, c, x[1], 12, 0xe8c7b756);
        FF(c, d, a, b, x[2], 17, 0x242070db); FF(b, c, d, a, x[3], 22, 0xc1bdceee);
        FF(a, b, c, d, x[4], 7, 0xf57c0faf); FF(d, a, b, c, x[5], 12, 0x4787c62a);
        FF(c, d, a, b, x[6], 17, 0xa8304613); FF(b, c, d, a, x[7], 22, 0xfd469501);
        FF(a, b, c, d, x[8], 7, 0x698098d8); FF(d, a, b, c, x[9], 12, 0x8b44f7af);
        FF(c, d, a, b, x[10], 17, 0xffff5bb1); FF(b, c, d, a, x[11], 22, 0x895cd7be);
        FF(a, b, c, d, x[12], 7, 0x6b901122); FF(d, a, b, c, x[13], 12, 0xfd987193);
        FF(c, d, a, b, x[14], 17, 0xa679438e); FF(b, c, d, a, x[15], 22, 0x49b40821);

        GG(a, b, c, d, x[1], 5, 0xf61e2562); GG(d, a, b, c, x[6], 9, 0xc040b340);
        GG(c, d, a, b, x[11], 14, 0x265e5a51); GG(b, c, d, a, x[0], 20, 0xe9b6c7aa);
        GG(a, b, c, d, x[5], 5, 0xd62f105d); GG(d, a, b, c, x[10], 9, 0x02441453);
        GG(c, d, a, b, x[15], 14, 0xd8a1e681); GG(b, c, d, a, x[4], 20, 0xe7d3fbc8);
        GG(a, b, c, d, x[9], 5, 0x21e1cde6); GG(d, a, b, c, x[14], 9, 0xc33707d6);
        GG(c, d, a, b, x[3], 14, 0xf4d50d87); GG(b, c, d, a, x[8], 20, 0x455a14ed);
        GG(a, b, c, d, x[13], 5, 0xa9e3e905); GG(d, a, b, c, x[2], 9, 0xfcefa3f8);
        GG(c, d, a, b, x[7], 14, 0x676f02d9); GG(b, c, d, a, x[12], 20, 0x8d2a4c8a);

        HH(a, b, c, d, x[5], 4, 0xfffa3942); HH(d, a, b, c, x[8], 11, 0x8771f681);
        HH(c, d, a, b, x[11], 16, 0x6d9d6122); HH(b, c, d, a, x[14], 23, 0xfde5380c);
        HH(a, b, c, d, x[1], 4, 0xa4beea44); HH(d, a, b, c, x[4], 11, 0x4bdecfa9);
        HH(c, d, a, b, x[7], 16, 0xf6bb4b60); HH(b, c, d, a, x[10], 23, 0xbebfbc70);
        HH(a, b, c, d, x[13], 4, 0x289b7ec6); HH(d, a, b, c, x[0], 11, 0xeaa127fa);
        HH(c, d, a, b, x[3], 16, 0xd4ef3085); HH(b, c, d, a, x[6], 23, 0x04881d05);
        HH(a, b, c, d, x[9], 4, 0xd9d4d039); HH(d, a, b, c, x[12], 11, 0xe6db99e5);
        HH(c, d, a, b, x[15], 16, 0x1fa27cf8); HH(b, c, d, a, x[2], 23, 0xc4ac5665);

        II(a, b, c, d, x[0], 6, 0xf4292244); II(d, a, b, c, x[7], 10, 0x432aff97);
        II(c, d, a, b, x[14], 15, 0xab9423a7); II(b, c, d, a, x[5], 21, 0xfc93a039);
        II(a, b, c, d, x[12], 6, 0x655b59c3); II(d, a, b, c, x[3], 10, 0x8f0ccc92);
        II(c, d, a, b, x[10], 15, 0xffeff47d); II(b, c, d, a, x[1], 21, 0x85845dd1);
        II(a, b, c, d, x[8], 6, 0x6fa87e4f); II(d, a, b, c, x[15], 10, 0xfe2ce6e0);
        II(c, d, a, b, x[6], 15, 0xa3014314); II(b, c, d, a, x[13], 21, 0x4e0811a1);
        II(a, b, c, d, x[4], 6, 0xf7537e82); II(d, a, b, c, x[11], 10, 0xbd3af235);
        II(c, d, a, b, x[2], 15, 0x2ad7d2bb); II(b, c, d, a, x[9], 21, 0xeb86d391);

        state[0] += a; state[1] += b; state[2] += c; state[3] += d;
    }

#undef FF
#undef GG
#undef HH
#undef II

public:
    MD5() : count(0), finalized(false) {
        state[0] = 0x67452301;
        state[1] = 0xefcdab89;
        state[2] = 0x98badcfe;
        state[3] = 0x10325476;
    }

    void update(const uint8_t* input, size_t length) {
        size_t index = (size_t)(count % 64);
        count += length;

        size_t i = 0;
        if (index) {
            size_t part = std::min((size_t)64 - index, length);
            memcpy(buffer + index, input, part);
            if (index + part < 64) return;
            transform(buffer);
            i = part;
        }

        for (; i + 64 <= length; i += 64)
            transform(input + i);

        if (i < length)
            memcpy(buffer, input + i, length - i);
    }

    void finalize() {
        if (finalized) return;
        finalized = true;

        uint64_t savedBits = count * 8;

        uint8_t padding[64];
        memset(padding, 0, 64);
        padding[0] = 0x80;

        size_t index = (size_t)(count % 64);
        size_t padLen = (index < 56) ? (56 - index) : (120 - index);
        update(padding, padLen);

        uint8_t bits_arr[8];
        for (int i = 0; i < 8; i++)
            bits_arr[i] = (uint8_t)((savedBits >> (i * 8)) & 0xff);
        update(bits_arr, 8);

        for (int i = 0; i < 4; i++) {
            digest[i * 4] = (uint8_t)(state[i] & 0xff);
            digest[i * 4 + 1] = (uint8_t)((state[i] >> 8) & 0xff);
            digest[i * 4 + 2] = (uint8_t)((state[i] >> 16) & 0xff);
            digest[i * 4 + 3] = (uint8_t)((state[i] >> 24) & 0xff);
        }
    }

    const uint8_t* getDigest() {
        finalize();
        return digest;
    }
};

// ==================== Hash Class ====================

class Hash {
public:
    static void showAlgorithms() {
        std::cout << "MD5" << std::endl;
        // 如需其他算法可在此添加
    }

private:
    std::string algorithm;

public:
    Hash(const std::string& algo) : algorithm(algo) {}

    // 输入字符串返回哈希值（与Java的BigInteger(1, bytes).toString(16)格式一致）
    std::string hashValue(const std::string& input) {
        if (algorithm == "MD5") {
            MD5 md5;
            md5.update(reinterpret_cast<const uint8_t*>(input.data()), input.size());
            const uint8_t* digest = md5.getDigest();

            static const char hexChars[] = "0123456789abcdef";
            std::string result;
            bool leading = true;
            for (int i = 0; i < 16; i++) {
                int high = (digest[i] >> 4) & 0x0f;
                int low = digest[i] & 0x0f;
                if (leading) {
                    if (high != 0) {
                        result += hexChars[high];
                        result += hexChars[low];
                        leading = false;
                    }
                    else if (low != 0) {
                        result += hexChars[low];
                        leading = false;
                    }
                    // 否则两个都是0，跳过前导零
                }
                else {
                    result += hexChars[high];
                    result += hexChars[low];
                }
            }
            if (result.empty()) result = "0";
            return result;
        }
        return "";
    }
};

// ==================== 字符串生成 ====================

void generateStringsHelper(const std::vector<char>& arr, int i, int n,
    std::string& path, std::vector<std::string>& ans) {
    if (i == n) {
        ans.push_back(path);
    }
    else {
        for (char cha : arr) {
            path[i] = cha;
            generateStringsHelper(arr, i + 1, n, path, ans);
        }
    }
}

std::vector<std::string> generateStrings(const std::vector<char>& arr, int n) {
    std::string path(n, ' ');
    std::vector<std::string> ans;
    generateStringsHelper(arr, 0, n, path, ans);
    return ans;
}

// ==================== 十六进制字符串取模 ====================

int hexMod(const std::string& hexStr, int m) {
    long long result = 0;
    for (char c : hexStr) {
        int digit;
        if (c >= '0' && c <= '9') digit = c - '0';
        else digit = c - 'a' + 10;
        result = (result * 16 + digit) % m;
    }
    return (int)result;
}

// ==================== Main ====================

int main() {
    std::cout << "支持的哈希算法 : " << std::endl;
    Hash::showAlgorithms();
    std::cout << std::endl;

    std::string algorithm = "MD5";
    Hash hash(algorithm);

    std::string str1 = "zuochengyunzuochengyunzuochengyun1";
    std::string str2 = "zuochengyunzuochengyunzuochengyun2";
    std::string str3 = "zuochengyunzuochengyunzuochengyun3";
    std::string str4 = "zuochengyunzuochengyunZuochengyun1";
    std::string str5 = "zuochengyunzuoChengyunzuochengyun2";
    std::string str6 = "zuochengyunzuochengyunzuochengyUn3";
    std::string str7 = "zuochengyunzuochengyunzuochengyun1";

    std::cout << "7个字符串得到的哈希值 : " << std::endl;
    std::cout << hash.hashValue(str1) << std::endl;
    std::cout << hash.hashValue(str2) << std::endl;
    std::cout << hash.hashValue(str3) << std::endl;
    std::cout << hash.hashValue(str4) << std::endl;
    std::cout << hash.hashValue(str5) << std::endl;
    std::cout << hash.hashValue(str6) << std::endl;
    std::cout << hash.hashValue(str7) << std::endl;
    std::cout << std::endl;

    std::vector<char> arr = { 'a', 'b' };
    int n = 20;
    std::cout << "生成长度为n，字符来自arr，所有可能的字符串" << std::endl;
    std::vector<std::string> strs = generateStrings(arr, n);
    std::cout << "不同字符串的数量 : " << strs.size() << std::endl;

    std::unordered_set<std::string> hashSet;
    for (const std::string& str : strs) {
        hashSet.insert(hash.hashValue(str));
    }
    std::cout << "不同哈希值的数量 : " << hashSet.size() << std::endl;
    std::cout << std::endl;

    int m = 13;
    std::vector<int> cnts(m, 0);
    std::cout << "现在看看这些哈希值，% " << m << " 之后的余数分布情况" << std::endl;
    for (const std::string& hashCode : hashSet) {
        int ans = hexMod(hashCode, m);
        cnts[ans]++;
    }
    for (int i = 0; i < m; i++) {
        std::cout << "余数 " << i << " 出现了 " << cnts[i] << " 次" << std::endl;
    }

    return 0;
}




//哈希函数相关问题：
//一台机器上硬盘空间很大，但是内存空间很少只有4G
//给定100亿个字符串的文件，每行是一个字符串长100字节，统计哪个字符串出现的次数最多
//不需要代码实现，聊清楚原理即可

//很多工程上的问题都是利用哈希函数把大数据量的样本均匀分散到多台机器上 或者 多个小文件里
//哈希函数可以保证同一个样本一定会放在一起，还可以保证把不同种类的样本均匀分开




//哈希表原理 & 哈希表扩容
//哈希表增删改查的均摊复杂度是O(k), k是样本平均长度
// long long 类型和 int 类型时复杂度为 O(1)

//如下的细节都可以定制：
//初始桶空间，一开始准备多少个桶？
//扩容阈值条件，链表长度多少时扩容？
//扩容因子，一次增加多少桶空间？
//哈希函数选择，简单的哈希函数 or 复杂的哈希函数？
//桶结构的具体实现，简单链表？开放地址？红黑树？

//不管定制什么样的细节，但是哈希表的原理是不变的
//所有的不同定制也仅仅是优化常数时间，时间复杂度无法再优化




//布隆过滤器原理  （位图）
//设计一个黑名单系统（爬虫去重系统），有100亿个url需要进入黑名单，每个url有100字节
//建立好黑名单系统后，可以判断任何一个url在不在黑名单内，预期失误率万分之一，内存占用不超过30G
//布隆过滤器不仅可以做黑名单、爬虫去重系统、还能做数据定位
//
//假设数据量为n，预期的失误率为p，布隆过滤器大小和每个样本的大小无关
//1.根据n和p，算出布隆过滤器一共需要多少个bit位，向上取整为m，注意m是bit数量，m/8才是字节数
//2.根据m和n，算出布隆过滤器应该选择多少个哈希函数，向上取整，记为k
//3.根据修正公式，算出真实的失误率p_true

// m = - (n*lnp)/((ln2)^2)
// k = ln2*(m/n)
// 真实失误率：(1 - e^(-n*k/m) )^k




//一致性哈希原理
//1.一种简单的存储结构介绍，弱点是 增加 or 减少机器，数据迁移的代价是全量的
//2.选择哈希key的注意点
//3.一致性哈希实现的分布式存储结构，哈希域变环、机器进环设计
//4.一致性哈希的虚拟节点技术可以规避数据倾斜、实现负载均衡、实现负载管理

