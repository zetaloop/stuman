// ==================== 数学处理函数 ====================

int _max(int *col) // 最大值
{
    if (datalen == 0)
        return 0;
    int max = INT_MIN;
    for (int i = 0; i < datalen; i++)
    {
        if (col[i] > max)
        {
            max = col[i];
        }
    }
    return max;
}

char *_cmax(int *col, char *buf)
{
    if (datalen == 0)
        return "NA";
    return int2str(_max(col), buf);
}

int _min(int *col) // 最小值
{
    if (datalen == 0)
        return 0;
    int min = INT_MAX;
    for (int i = 0; i < datalen; i++)
    {
        if (col[i] < min)
        {
            min = col[i];
        }
    }
    return min;
}

char *_cmin(int *col, char *buf)
{
    if (datalen == 0)
        return "NA";
    return int2str(_min(col), buf);
}

double _mean(int *col) // 平均数
{
    if (datalen == 0)
        return 0;
    double sum = 0;
    for (int i = 0; i < datalen; i++)
    {
        sum += col[i];
    }
    return sum / datalen;
}

char *_cmean(int *col, char *buf)
{
    if (datalen == 0)
        return "NA";
    return double2str(_mean(col), 2, buf);
}

void _swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int _cmedian_quicksort(int arr[], int low, int high, int k)
{
    if (low < high)
    {
        int mid = low + (high - low) / 2;
        if (arr[low] > arr[mid])
            _swap(arr + low, arr + mid);
        if (arr[low] > arr[high])
            _swap(arr + low, arr + high);
        if (arr[mid] > arr[high])
            _swap(arr + mid, arr + high);
        _swap(arr + mid, arr + high - 1);
        int pivot = arr[high - 1];
        int i = low, j = high - 1;
        for (;;)
        {
            while (arr[++i] < pivot)
            {
            }
            while (arr[--j] > pivot)
            {
            }
            if (i < j)
                _swap(arr + i, arr + j);
            else
                break;
        }
        _swap(arr + i, arr + high - 1);
        if (i == k)
            return arr[i];
        else if (i > k)
            return _cmedian_quicksort(arr, low, i - 1, k);
        else
            return _cmedian_quicksort(arr, i + 1, high, k);
    }
    return arr[low];
}

char *_cmedian(int *col, char *buf) // 中位数
{
    if (datalen == 0)
        return "NA";
    int *temp = (int *)malloc(datalen * sizeof(int));
    for (int i = 0; i < datalen; i++)
        temp[i] = col[i];
    int median;
    if (datalen % 2 == 0)
    {
        int mid1 = _cmedian_quicksort(temp, 0, datalen - 1, datalen / 2);
        int mid2 = _cmedian_quicksort(temp, 0, datalen - 1, datalen / 2 - 1);
        free(temp);
        median = (mid1 + mid2) / 2;
        return double2str(median, 1, buf);
    }
    else
    {
        median = _cmedian_quicksort(temp, 0, datalen - 1, datalen / 2);
        free(temp);
        return int2str(median, buf);
    }
}

void _cmode_quicksort(int arr[], int low, int high)
{
    if (low < high)
    {
        int mid = low + (high - low) / 2;
        if (arr[low] > arr[mid])
            _swap(arr + low, arr + mid);
        if (arr[low] > arr[high])
            _swap(arr + low, arr + high);
        if (arr[mid] > arr[high])
            _swap(arr + mid, arr + high);
        _swap(arr + mid, arr + high - 1);
        int pivot = arr[high - 1];
        int i = low, j = high - 1;
        for (;;)
        {
            while (arr[++i] < pivot)
            {
            }
            while (arr[--j] > pivot)
            {
            }
            if (i < j)
                _swap(arr + i, arr + j);
            else
                break;
        }
        _swap(arr + i, arr + high - 1);
        _cmode_quicksort(arr, low, i - 1);
        _cmode_quicksort(arr, i + 1, high);
    }
}

char *_cmode(int *col, char *buf) // 众数
{
    if (datalen == 0)
        return "NA";
    int *temp = (int *)malloc(datalen * sizeof(int));
    memcpy(temp, col, datalen * sizeof(int));
    _cmode_quicksort(temp, 0, datalen - 1);
    int maxCount = 0, currentCount = 0;
    char result[MAX_NAMECHAR] = {0};
    size_t resultSize = 0;
    for (int i = 0; i < datalen; i++)
    {
        currentCount++;
        if (i == datalen - 1 || temp[i] != temp[i + 1])
        {
            if (currentCount > maxCount)
            {
                // 发现了新的众数
                maxCount = currentCount;
                char *modeStr = int2str(temp[i], buf);
                strncpy(result, modeStr, sizeof(result) - 1);
                resultSize = strlen(result) + 1;
            }
            else if (currentCount == maxCount)
            {
                // 发现了另一个众数
                char *modeStr = int2str(temp[i], buf);
                size_t newSize = resultSize + strlen(modeStr) + 3; // 额外的空间用于逗号和null终结符
                if (newSize < sizeof(result))
                {
                    strcat(result, ", ");
                    strcat(result, modeStr);
                    resultSize = newSize;
                }
            }
            currentCount = 0;
        }
    }
    free(temp);
    strncpy(buf, result, sizeof(result)); // 将结果复制到buf
    return buf;
}

double _variance(int *col) // 方差
{
    if (datalen == 0)
        return 0;
    double mean = _mean(col);
    double sum = 0;
    for (int i = 0; i < datalen; i++)
    {
        sum += (col[i] - mean) * (col[i] - mean);
    }
    return sum / datalen;
}

char *_cvariance(int *col, char *buf)
{
    if (datalen == 0)
        return "NA";
    return double2str(_variance(col), 2, buf);
}

double _stddev(int *col) // 标准差
{
    return sqrt(_variance(col));
}

char *_cstddev(int *col, char *buf)
{
    if (datalen == 0)
        return "NA";
    return double2str(_stddev(col), 2, buf);
}

char *_crange(int *col, char *buf) // 极差
{
    if (datalen == 0)
        return "NA";
    return int2str(_max(col) - _min(col), buf);
}

char *_cskewness(int *col, char *buf) // 偏度
{
    if (datalen == 0)
        return "NA";
    double mean = _mean(col);
    double stddev = _stddev(col);
    double sum = 0;
    for (int i = 0; i < datalen; i++)
    {
        sum += (col[i] - mean) * (col[i] - mean) * (col[i] - mean);
    }
    return double2str(sum / (datalen * stddev * stddev * stddev), 4, buf);
}

char *_ckurtosis(int *col, char *buf) // 峰度
{
    if (datalen == 0)
        return "NA";
    double mean = _mean(col);
    double stddev = _stddev(col);
    double sum = 0;
    for (int i = 0; i < datalen; i++)
    {
        sum += (col[i] - mean) * (col[i] - mean) * (col[i] - mean) * (col[i] - mean);
    }
    return double2str(sum / (datalen * stddev * stddev * stddev * stddev), 4, buf);
}

//========SORTER========
void swap(int i, int j)
{
    unsigned long long int t1 = ID[i];
    ID[i] = ID[j];
    ID[j] = t1;

    char t2[MAX_NAMECHAR];
    strcpy(t2, name[i]);
    strcpy(name[i], name[j]);
    strcpy(name[j], t2);

    int t3 = maths[i];
    maths[i] = maths[j];
    maths[j] = t3;

    int t4 = physc[i];
    physc[i] = physc[j];
    physc[j] = t4;

    int t5 = clang[i];
    clang[i] = clang[j];
    clang[j] = t5;

    int t6 = total[i];
    total[i] = total[j];
    total[j] = t6;
}

void _sortpre() // 头尾交换倒排
{
    for (int i = 0; i < datalen / 2; i++)
    {
        swap(i, datalen - i - 1);
    }
}

int partitionBy(int *by, int low, int high, int reverse)
{
    // 三数取中法选择枢纽
    int mid = low + (high - low) / 2;
    if (reverse ? (by[low] < by[mid]) : (by[low] > by[mid]))
        swap(low, mid);
    if (reverse ? (by[low] < by[high]) : (by[low] > by[high]))
        swap(low, high);
    if (reverse ? (by[mid] < by[high]) : (by[mid] > by[high]))
        swap(mid, high);

    int pivot = by[mid];
    swap(mid, high); // 将枢纽移到尾部

    int i = (low - 1);
    for (int j = low; j <= high - 1; j++)
    {
        int cmp1 = by[j] - pivot;
        int cmp2 = ID[j] - ID[high];
        if (reverse)
            cmp2 = -cmp2;
        if (reverse ? (cmp1 > 0 || (cmp1 == 0 && cmp2 > 0)) : (cmp1 < 0 || (cmp1 == 0 && cmp2 < 0)))
        {
            i++;
            swap(i, j);
        }
    }
    swap(i + 1, high);
    return (i + 1);
}

void quickSortBy(int *by, int low, int high, int reverse)
{
    if (low < high)
    {
        int pi = partitionBy(by, low, high, reverse);
        quickSortBy(by, low, pi - 1, reverse);
        quickSortBy(by, pi + 1, high, reverse);
    }
}

void _sortby(int *by, int reverse)
{
    quickSortBy(by, 0, datalen - 1, reverse);
}

int partitionById(int low, int high, int reverse)
{
    // 三数取中法选择枢纽
    int mid = low + (high - low) / 2;
    if (reverse ? (ID[low] < ID[mid]) : (ID[low] > ID[mid]))
        swap(low, mid);
    if (reverse ? (ID[low] < ID[high]) : (ID[low] > ID[high]))
        swap(low, high);
    if (reverse ? (ID[mid] < ID[high]) : (ID[mid] > ID[high]))
        swap(mid, high);

    unsigned long long int pivot = ID[mid];
    swap(mid, high); // 将枢纽移到尾部

    int i = (low - 1);
    for (int j = low; j <= high - 1; j++)
    {
        if (reverse ? (ID[j] > pivot) : (ID[j] < pivot))
        {
            i++;
            swap(i, j);
        }
    }
    swap(i + 1, high);
    return (i + 1);
}

void quickSortById(int low, int high, int reverse)
{
    if (low < high)
    {
        int pi = partitionById(low, high, reverse);
        quickSortById(low, pi - 1, reverse);
        quickSortById(pi + 1, high, reverse);
    }
}

void _sortbyid(int reverse)
{
    quickSortById(0, datalen - 1, reverse);
}

int partitionByName(int low, int high, int reverse)
{
    // 三数取中法选择枢纽
    int mid = low + (high - low) / 2;
    if (strcmp(name[low], name[mid]) > 0)
        swap(low, mid);
    if (strcmp(name[low], name[high]) > 0)
        swap(low, high);
    if (strcmp(name[mid], name[high]) > 0)
        swap(mid, high);

    char *pivot = name[mid];
    swap(mid, high); // 将枢纽移到尾部

    int i = (low - 1);
    for (int j = low; j <= high - 1; j++)
    {
        int cmp1 = strcmp(name[j], pivot);
        int cmp2 = ID[j] - ID[high];
        if (reverse)
            cmp2 = -cmp2;
        if (reverse ? (cmp1 > 0 || (cmp1 == 0 && cmp2 > 0)) : (cmp1 < 0 || (cmp1 == 0 && cmp2 < 0)))
        {
            i++;
            swap(i, j);
        }
    }
    swap(i + 1, high);
    return (i + 1);
}

void quickSortByName(int low, int high, int reverse)
{
    if (low < high)
    {
        int pi = partitionByName(low, high, reverse);
        quickSortByName(low, pi - 1, reverse);
        quickSortByName(pi + 1, high, reverse);
    }
}

void _sortbyname(int reverse)
{
    quickSortByName(0, datalen - 1, reverse);
}

// old bubble sort
/*
void _sortby(int *by, int reverse) // 排序
{
    for (int i = 0; i < datalen; i++)
    {
        for (int j = 0; j < datalen - i - 1; j++)
        {
            if (reverse ? (by[j] < by[j + 1]) : (by[j] > by[j + 1]))
            {
                unsigned long long int t1 = ID[j];
                ID[j] = ID[j + 1];
                ID[j + 1] = t1;

                char t2[MAX_NAMECHAR];
                strcpy(t2, name[j]);
                strcpy(name[j], name[j + 1]);
                strcpy(name[j + 1], t2);

                int t3 = maths[j];
                maths[j] = maths[j + 1];
                maths[j + 1] = t3;

                int t4 = physc[j];
                physc[j] = physc[j + 1];
                physc[j + 1] = t4;

                int t5 = clang[j];
                clang[j] = clang[j + 1];
                clang[j + 1] = t5;

                int t6 = total[j];
                total[j] = total[j + 1];
                total[j + 1] = t6;
            }
        }
    }
}

void _sortbyid(int reverse) // 学号排序(llu)
{
    for (int i = 0; i < datalen; i++)
    {
        for (int j = 0; j < datalen - i - 1; j++)
        {
            if (reverse ? (ID[j] < ID[j + 1]) : (ID[j] > ID[j + 1]))
            {
                unsigned long long int t1 = ID[j];
                ID[j] = ID[j + 1];
                ID[j + 1] = t1;

                char t2[MAX_NAMECHAR];
                strcpy(t2, name[j]);
                strcpy(name[j], name[j + 1]);
                strcpy(name[j + 1], t2);

                int t3 = maths[j];
                maths[j] = maths[j + 1];
                maths[j + 1] = t3;

                int t4 = physc[j];
                physc[j] = physc[j + 1];
                physc[j + 1] = t4;

                int t5 = clang[j];
                clang[j] = clang[j + 1];
                clang[j + 1] = t5;

                int t6 = total[j];
                total[j] = total[j + 1];
                total[j + 1] = t6;
            }
        }
    }
}

void _sortbyname(int reverse) // 姓名排序
{
    for (int i = 0; i < datalen; i++)
    {
        for (int j = 0; j < datalen - i - 1; j++)
        {
            if (reverse ? (strcoll(name[j], name[j + 1]) < 0) : (strcoll(name[j], name[j + 1]) > 0))
            {
                unsigned long long int t1 = ID[j];
                ID[j] = ID[j + 1];
                ID[j + 1] = t1;

                char t2[MAX_NAMECHAR];
                strcpy(t2, name[j]);
                strcpy(name[j], name[j + 1]);
                strcpy(name[j + 1], t2);

                int t3 = maths[j];
                maths[j] = maths[j + 1];
                maths[j + 1] = t3;

                int t4 = physc[j];
                physc[j] = physc[j + 1];
                physc[j + 1] = t4;

                int t5 = clang[j];
                clang[j] = clang[j + 1];
                clang[j + 1] = t5;

                int t6 = total[j];
                total[j] = total[j + 1];
                total[j + 1] = t6;
            }
        }
    }
}*/