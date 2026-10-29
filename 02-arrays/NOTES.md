# 02 – 陣列與指標

日期：2026-10-29 ~ 10-30
檔案：`arrays.c`

---

## 題目 1：指標 +1 會跳多遠

```c
int    n = 10;   int    *p = &n;
char   c = 'A';  char   *q = &c;
double d = 1.5;  double *r = &d;
printf("p+1: %p\n", (void*)(p + 1));
```

| 指標 | 原本 | +1 之後 | 差 |
|---|---|---|---|
| `int *p` | `...d4` | `...d8` | 4 |
| `char *q` | `...d3` | `...d4` | 1 |
| `double *r` | `...d8` | `...e0` | 8 |

**結論：指標 +1 = 往後跳「一個該型別的大小」，不是位址 +1。**

- 位址是 16 進位：`d8` + 8 = `e0`
- 變數在 stack 上剛好排在一起（`q+1` 剛好等於 `p`），但**不能依賴這種排法**，那是未定義行為。

---

## 題目 2：三種寫法走訪陣列

```c
int arr[5] = {10, 20, 30, 40, 50};
for (size_t i = 0; i < sizeof(arr) / sizeof(arr[0]); i++) {
    printf("i: %zu  arr[i]: %d  *(arr+i): %d  &arr[i]: %p\n",
           i, arr[i], *(arr + i), (void*)&arr[i]);
}
```

```
&arr[i]: 0x7fffe3904130 → 134 → 138 → 13c → 140   每次差 4
```

- **`arr[i]` 就是 `*(arr + i)` 的簡寫**，兩者完全相同。
- `arr` 本身代表陣列開頭的位址，和 `&arr[0]` 一樣。
- `arr + i` 往後跳 `i × sizeof(int)` = `i × 4` bytes。

```
*(arr + i)
   arr       陣列開頭的位址
   arr + i   往後跳 i 個 int
   *( )      到那個位址取值
```

**`i` 是 `size_t`（8 bytes）不影響跳幾格。** i 的大小只決定它能存多大的數；跳多遠只看 arr 指向的型別。

### 陣列長度：sizeof 慣用寫法

```c
sizeof(arr)                    // 整個陣列幾 bytes = 20
sizeof(arr[0])                 // 一個元素幾 bytes = 4
sizeof(arr) / sizeof(arr[0])   // 元素個數 = 5
```

**`sizeof` 永遠回傳 bytes 數，不是元素個數。** 就像一盒蛋 300g ÷ 一顆 60g = 5 顆。

---

## 意外收穫：陣列越界

迴圈條件不小心寫成 `i < sizeof(arr)`（= 20），結果讀到 i = 19：

```
i: 4   arr[i]: 50            ← 陣列最後一格
i: 5   arr[i]: 32767         ← 已經超出陣列
i: 6   arr[i]: 1961596416
...
```

- **C 不會檢查陣列範圍**，超出範圍不會報錯，直接讀到後面記憶體裡的東西。
- `32767` = `0x7fff`，很可能是某個 stack 位址（`0x7fff...`）的上半部。
- **讀**越界 → 垃圾值，邏輯默默出錯。
- **寫**越界 → 蓋掉其他變數或返回位址，就是 **buffer overflow** 漏洞。
- 用 AddressSanitizer 可以抓到：`gcc -g -fsanitize=address ...` → `stack-buffer-overflow`

---

## 題目 3：sizeof 陷阱（陣列傳進函式）

```c
void print_size(int a[]) {
    printf("in function: %zu\n", sizeof(a));
}

int *ptr = arr;
sizeof(arr);       // 20
sizeof(ptr);       // 8
print_size(arr);   // 8 ← !!
```

| | 結果 | 原因 |
|---|---|---|
| `sizeof(arr)` | 20 | 真正的陣列 |
| `sizeof(ptr)` | 8 | 指標，64 位元系統上是 8 bytes |
| 函式裡 `sizeof(a)` | 8 | **陣列傳進函式會變成指標（decay）** |

- 參數寫 `int a[]`，編譯器看到的是 `int *a`。
- gcc 會警告：`'sizeof' on array function parameter 'a' will return size of 'int *'`
- 所以 `sizeof(arr) / sizeof(arr[0])` **在函式裡會失靈**（8 ÷ 4 = 2）。

**實務寫法：長度要一起傳進去**

```c
void print_all(const int *a, size_t n);
print_all(arr, sizeof(arr) / sizeof(arr[0]));
```

`memcpy(dst, src, n)`、`read(fd, buf, n)` 都是這樣設計的。C++ 的 `std::vector` 會自己記長度，就是為了解決這個問題。

---

## 題目 4：用一維陣列存二維資料

```c
#define H 3
#define W 4
int img[H * W];

for (size_t i = 0; i < H; i++)
    for (size_t j = 0; j < W; j++)
        img[i * W + j] = i * 10 + j;
```

```
0 1 2 3
10 11 12 13
20 21 22 23
```

**(i, j) 在一維陣列的位置 = `i * W + j`**：前面有 i 整列，每列 W 格，再加上這列的第 j 格。

- 所有影像在記憶體裡都是這樣攤平成一維的（OpenCV 的 `cv::Mat` 也是）。
- video-infer 裡 HWC → CHW 的公式就是這個的延伸：
  - HWC：`(y * W + x) * 3 + c`
  - CHW：`c * H * W + y * W + x`
- 用 `H`、`W` 不要寫死數字：寫死 `i * 4` 的話，改了 W 就會默默算錯位置。

---

## 型別大小

**1 byte = 8 bits，可以表示 2⁸ = 256 種值。** bytes 之間是**相乘**，不是相加：

| 型別 | bytes | 範圍 |
|---|---|---|
| `char` | 1 | -128 ~ 127（ASCII 只用到 128 個，`'A'` = 65） |
| `short` | 2 | 約 ±3 萬 |
| `int` | 4 | 2³² ≈ 43 億種，約 ±21 億 |
| `long long` | 8 | 約 ±922 京 |
| `float` | 4 | 約 7 位有效數字 |
| `double` | 8 | 約 15 位有效數字 |

就像十進位：4 位數是 10 × 10 × 10 × 10 = 10000 種，不是 10 × 4 = 40 種。

**C 標準沒有規定 int 一定是 4 bytes**（只保證至少 2）。韌體常用 `<stdint.h>` 的固定大小型別：

```c
uint8_t   // 1 byte，0 ~ 255（影像像素常用）
int32_t   // 4 bytes
uint32_t  // 4 bytes，無號
int64_t   // 8 bytes（ONNX Runtime 的 shape 就是這個）
```

---

## 8 / 16 / 32 / 64 位元

**N 位元 CPU = 暫存器有 N bits 寬**，也就是 CPU 一次能處理多大的數字。

| CPU | 一次處理 | 例子 |
|---|---|---|
| 8 位元 | 1 byte | 8051、Arduino Uno（AVR） |
| 16 位元 | 2 bytes | MSP430 |
| 32 位元 | 4 bytes | STM32（ARM Cortex-M） |
| 64 位元 | 8 bytes | 現在的電腦、手機、樹莓派 3 之後 |

- 位址也存在暫存器裡 → **位元數決定能用多少記憶體**：32 位元最多 2³² = 4 GB。
- **指標的大小 = 位址的大小**：64 位元系統 `sizeof(指標)` = 8，32 位元 = 4。
- 樹莓派 3B+ 的 CPU 是 64 位元，要裝 64 位元 OS 才能用 ONNX Runtime 的 ARM64 版本。

---

## 我犯的錯

### 1. `(void*)p+1`：轉型的優先順序比 `+` 高
```c
(void*)p+1     // ✗ 先轉成 void* 再 +1 → 只跳 1 byte
(void*)(p+1)   // ✓ 先用 int* 的規則 +1，再轉型
```
三組都只差 1 → 量到的根本不是 int* / char* / double* 的 +1。

### 2. 用 `%d` 印 char
```c
printf("%d", c);   // 65（char 其實就是數字）
printf("%c", c);   // A
```

### 3. C 的陣列沒有 `.shape()` / `.size()`
```
error: request for member 'shape' in something not a structure or union
```
`.` 只能用在 struct。C 陣列只是一排記憶體，長度要自己知道。

### 4. `size_t` 要用 `%zu`
```c
printf("%d", i);    // ✗ warning：size_t 是 8 bytes
printf("%zu", i);   // ✓
```

### 5. 印位址少了 `&`（跟 swap 時一樣的錯）
```c
(void*)arr[i]    // ✗ 把值 10 當成位址
(void*)&arr[i]   // ✓
```
warning：`cast to pointer from integer of different size`

### 6. 迴圈條件寫成 `i < sizeof(arr)`
→ 跑了 20 次，讀到陣列外面（見「陣列越界」）。

### 7. `img[i,j]`：Python 的習慣
C 沒有多維索引語法。逗號是運算子，`img[i,j]` 等於 **`img[j]`**，i 被丟掉 → 三列都寫進前 4 格。
**修正**：`img[i * W + j]`

### 8. `printf("%d", img)` 想印整個陣列
陣列會退化成位址。C 沒辦法一次印整個陣列，要用迴圈一格一格印。

---

## 我的理解（自己寫）

**Q：為什麼 `arr[i]` 和 `*(arr+i)` 一樣？**

A：

**Q：為什麼陣列傳進函式之後，sizeof 變成 8？要怎麼解決？**

A：