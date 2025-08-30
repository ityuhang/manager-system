#ifndef HASH_TABLE_H
#define HASH_TABLE_H

// 声明哈希节点
typedef struct HashNode
{
    char* key;
    void* value;
    struct HashNode* next;
    
} HashNode;

// 声明哈希结构体
typedef struct HashTable
{
    HashNode** buckets;
    size_t size;
    size_t capacity;
} HashTable;


// 创建哈希表
HashTable* hash_table(size_t capacity);
//插入元素
void hash_insert(HashTable* table, const char* key, void* value);
// 删除
int hash_del(HashTable* table, const char* key);
// 查找
void* hash_get(HashTable* table, const char* key);
// 遍历哈希表
void hash_iterate(HashTable* table, void (*func)(const char*, void*));
// 销毁哈希表
void destroy_hash_table(HashTable* table);

#endif