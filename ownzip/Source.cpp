#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdlib>
#include <vector>
using namespace std;
#define MAX_TREE_HT 100


struct Huffman_codes
{
    unsigned char elem;
    std::vector<int> cod;
};
std::vector<int> temporary;
Huffman_codes code_table[512];
int members = 0;
int table[512];
unsigned char v_chars[512];

struct MinHeapNode {
    unsigned char data;
    unsigned freq;
    struct MinHeapNode* left, * right;
};
struct MinHeap {

    unsigned size;
    unsigned capacity;
    struct MinHeapNode** array;
};

struct MinHeapNode* newNode(unsigned char data, unsigned freq)
{
    struct MinHeapNode* temp
        = (struct MinHeapNode*)malloc
        (sizeof(struct MinHeapNode));

    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;

    return temp;
}


struct MinHeap* createMinHeap(unsigned capacity)

{

    struct MinHeap* minHeap
        = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->size = 0;

    minHeap->capacity = capacity;

    minHeap->array
        = (struct MinHeapNode**)malloc(minHeap->
            capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}
void swapMinHeapNode(struct MinHeapNode** a,
    struct MinHeapNode** b)
{
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}
void minHeapify(struct MinHeap* minHeap, int idx)
{
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->
        freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->
        freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest],
            &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}
int isSizeOne(struct MinHeap* minHeap)
{

    return (minHeap->size == 1);
}
struct MinHeapNode* extractMin(struct MinHeap* minHeap)

{

    struct MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0]
        = minHeap->array[minHeap->size - 1];

    --minHeap->size;
    minHeapify(minHeap, 0);

    return temp;
}

void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode)
{
    ++minHeap->size;
    int i = minHeap->size - 1;

    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {

        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }

    minHeap->array[i] = minHeapNode;
}
void buildMinHeap(struct MinHeap* minHeap)

{

    int n = minHeap->size - 1;
    int i;

    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}
void printArr(int arr[], int n)
{
    int i;
    for (i = 0; i < n; ++i)
    {
        printf("%d", arr[i]);
    }

    cout << "\n";
}
int isLeaf(struct MinHeapNode* root)
{
    return !(root->left) && !(root->right);
}
struct MinHeap* createAndBuildMinHeap(unsigned char data[], int freq[], int size)
{
    struct MinHeap* minHeap = createMinHeap(size);
    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);
    minHeap->size = size;
    buildMinHeap(minHeap);
    return minHeap;
}
struct MinHeapNode* buildHuffmanTree(unsigned char data[], int freq[], int size)

{
    struct MinHeapNode* left, * right, * top;

    // Step 1: Create a min heap of capacity
    // equal to size. Initially, there are
    // modes equal to size.
    struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);

    // Iterate while size of heap doesn't become 1
    while (!isSizeOne(minHeap)) {

        // Step 2: Extract the two minimum
        // freq items from min heap
        left = extractMin(minHeap);
        right = extractMin(minHeap);

        // Step 3: Create a new internal
        // node with frequency equal to the
        // sum of the two nodes frequencies.
        // Make the two extracted node as
        // left and right children of this new node.
        // Add this node to the min heap
        // '$' is a special value for internal nodes, not used
        top = newNode('$', left->freq + right->freq);

        top->left = left;
        top->right = right;

        insertMinHeap(minHeap, top);
    }

    // Step 4: The remaining node is the
    // root node and the tree is complete.
    return extractMin(minHeap);
}
void printCodes(struct MinHeapNode* root, int arr[], int top)

{
    if (root->left) {

        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }
    if (root->right) {

        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }

    if (isLeaf(root)) {
        for (int j = 0; j < top; j++)
        {
            temporary.push_back(arr[j]);
        }
        code_table[members].elem = root->data;
        code_table[members].cod = temporary;
        members++;
        temporary.clear();
        printf("%d : ", root->data);
        printArr(arr, top);
    }
}
bool* new_file;
int index_new = 0;
void archive(unsigned char* v, int len)
{
    FILE* a = fopen("result.myzip", "wb");
    unsigned char byte = 0;
    bool used;
    fwrite(&members, sizeof(int), sizeof(members), a);
    for (int i = 0; i < members; i++)
    {
        byte = code_table[i].elem;
        fwrite(&byte, sizeof(unsigned char), sizeof(byte), a);
        byte = code_table[i].cod.size();
        fwrite(&byte, sizeof(unsigned char), sizeof(byte), a);
        int count = 0;
        byte = 0;
        for (int j = 0; j < code_table[i].cod.size(); j++)
        {
            count++;
            byte = byte >> 1;
            if  (code_table[i].cod[j]==1)
            {
                byte += 128;
            }
            if (count == 8 || j== code_table[i].cod.size()-1)
            {
                count = 0;
                fwrite(&byte, sizeof(unsigned char), sizeof(byte), a);
                byte = 0;
            }
        }
    }
    new_file = new bool[8 * len + 1];
    for (int i = 0; i < len; i++)
    {
        used = false;
        int index = -1;
        int code_index = -1;
        for (int j = 0; j < members; j++)
        {
            if (code_table[j].elem == v[i])
            {
                index = j;
                break;
            }
        }
        for (int j = 0; j < code_table[index].cod.size(); j++)
        {
            if (code_table[index].cod[j] == 0)
            {
                /* new_file.push_back(false);*/
                new_file[index_new++] = 0;
            }
            else
            {
                new_file[index_new++] = 1;
            }
        }
    }
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            cout << new_file[8 * i + j];
        }
        cout << " ";
    }
    int counter = 0;
    for (int i = 0; i < index_new; i++)
    {
        byte = byte >> 1;
        if (new_file[i])
        {
            byte += 128;
        }
        counter++;
        if (counter == 8)
        {
            fwrite(&byte, sizeof(unsigned char), sizeof(byte), a);
            counter = 0;
            byte = 0;
        }
    }
    fclose(a);
    delete[]new_file;
}
void unarchive(const char* name1,const char* name)
{
    FILE* f = fopen(name1, "rb");
    FILE* a = fopen(name, "wb");
    unsigned char byte = 0;
    fseek(f, 0, SEEK_END);
    int len = ftell(f);
    unsigned char* vector = new unsigned char[len + 1];
    fseek(f, 0, SEEK_SET);


    fread(&members, sizeof(int), sizeof(members), f);
    for(int m=0;m<members;m++)
    {
        fread(&byte, sizeof(unsigned char), sizeof(byte), f);
        code_table[m].elem = byte;
        fread(&byte, sizeof(unsigned char), sizeof(byte), f);
        int copy = byte;
        code_table[m].cod.clear();
        int it = 0;
      while(copy>0)
      {
            fread(&byte, sizeof(unsigned char), sizeof(byte), f);
            for (int k = 0; k < 8; k++)
            {
                if ((byte & 128) == 128)
                {
                    code_table[m].cod.push_back(1);
                    copy--;
                }
                else
                {
                    copy--;
                    code_table[m].cod.push_back(0);
                }
                byte=byte << 1;
                if (copy == 0)
                    break;
            }
            reverse(code_table[m].cod.begin()+it*8, code_table[m].cod.end());
            it++;
        }
    }
    int offset = ftell(f);
    len -= offset;
    ////////////////////////
    fread(vector, 1, len, f);
    vector[len] = 0;
    new_file = new bool[8 * len + 1];
    for (int i = 0; i < len; i++)
    {
        unsigned char aux = vector[i];
        for (int k = 0; k < 8; k++)
        {
            if(aux%2==0)
            new_file[8 * i + k] = 0;
            else
            new_file[8 * i + k] = true;

            aux /= 2;
        }
    }
   /* for (int i=0 ; i < 100; i++)
    {
        for (int j=0; j < 8; j++)
        {
            cout << new_file[8 * i + j];
        }
        cout << " ";
    }*/
    int lung = 0;
    int i = 0;
    while(i<8*len)
    {
        //cout << i << endl;
        lung++;
        int index = -1;
        for (int k = 0; k < members; k++)
        {
            int ok = 1;
            if (lung == code_table[k].cod.size())
            {
              
                for (int l = 0; l < lung; l++)
                {
                    if (code_table[k].cod[l] != new_file[i + l])
                    {
                        ok = -1;
                        break;
                    }
                }
            }
            else
            {
                continue;
            }
            if (ok!=-1)
            {
                index = k;
                break;
            }

        }
        if (index == -1 && lung>30)
        {
            break;
        }
        if (index>=0)
        {
            i += lung;
            byte = code_table[index].elem;
            fwrite(&byte, sizeof(unsigned char), sizeof(byte), a);
            lung = 0;
        }
    }
    delete[]new_file;
    fclose(f);
    fclose(a);
   
}
void HuffmanCodes(unsigned char data[], int freq[], int size)
{
    struct MinHeapNode* root
        = buildHuffmanTree(data, freq, size);
    int arr[MAX_TREE_HT], top = 0;

    printCodes(root, arr, top);
}

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        cout << "Comanda gresita. -a file 1  || -u file1 file2";
        return 1;
    }
   
  
    if (strcmp(argv[1], "-a") == 0)
    {
        FILE* f = fopen(argv[2], "rb");
        if (f == NULL)
        {
            return 2;
        }
        fseek(f, 0, SEEK_END);
        int lung = ftell(f);
        unsigned char* vector = new unsigned char[lung + 1];
        fseek(f, 0, SEEK_SET);
        fread(vector, 1, lung, f);
        vector[lung] = 0;
        for (int i = 0; i < 512; i++)
        {
            table[i] = 0;

        }
        for (int i = 0; i < lung; i++)
        {
            table[vector[i]]++;
            v_chars[vector[i]] = vector[i];
        }
        /*double sum = 0;*/
        //for (int i = 0; i < 512; i++)
        //{
        //    if (table[i])
        //    {
        //        cout << i << ": " << table[i] << endl;
        //        /*table[i] /= lung;
        //        cout << i << ": " << table[i] << endl;*/
        //        //sum += table[i];
        //    }
        //}

        int real_size = 1;
        unsigned char v_chars_fin[512];
        int freq_table[512];
        v_chars_fin[0] = v_chars[0];
        freq_table[0] = table[0];
        for (int i = 0; i < 512; i++)
        {
            if (v_chars[i] != 0)
            {
                freq_table[real_size] = table[i];
                v_chars_fin[real_size] = v_chars[i];
                code_table[real_size].elem = v_chars[i];
                real_size++;
            }
        }
        HuffmanCodes(v_chars_fin, freq_table, real_size);
        cout << "START";
        /*for (int i = 0; i < members; i++)
        {
            printf("%d :", code_table[i].elem);
            for (auto each : code_table[i].cod)
            {
                cout << each;
            }
            cout << endl;
        }*/
        archive(vector, lung);
    }
    if(strcmp(argv[1], "-u")==0)
    unarchive(argv[2], argv[3]);
    return 0;
}