#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

// 创建哈希表
HashTable* hash_table(size_t capacity)
{
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));

    if(!table)
    {
        return NULL;
    }

    table->buckets = (HashNode**)calloc(capacity, sizeof(HashNode*));

    if(!table->buckets)
    {
        free(table);
        return NULL;
    }

    table->size = 0;
    table->capacity = capacity;

    return table;
}

// 哈希函数
static size_t hash_function(const char* key, size_t capacity)
{
    // 假如 key 为 "abc"，那么
	// r = ('a' * 31^2 + 'b' * 31^1 + 'c' * 31^0) % capacity;
    
    size_t r = 0, i;
    for (i = 0; key[i] != '\0'; i++)
    {
        r = r * 31 + key[i]; 
    }

    r %= capacity;

    return r;
    
}



//插入元素
void hash_insert(HashTable* table, const char* key, void* value)
{
    size_t pos = hash_function(key, table->capacity);

    HashNode* p = table->buckets[pos];

    while (p != NULL)
    {
        // 如果 key 存在，更新 value
        if(strcmp(p->key, key) == 0)
        {
            p->value = value;
            return;
        }

        p = p->next;
    }

    // key 不存在
    p = malloc(sizeof(HashNode));

    if(p == NULL)
    {
        perror("malloc");
        return;
    }

    p->key =  strdup(key);
    p->value = value;
    p->next = table->buckets[pos];

    table->buckets[pos] = p;
    table->size++;
    
}


// 删除
int hash_del(HashTable* table, const char* key)
{
    size_t pos = hash_function(key, table->capacity);

    HashNode* p = table->buckets[pos], *q = NULL;

    while (p != NULL)
    {
        // 找到了 key 
        if(strcmp(p->key, key) == 0)
        {
            if(q != NULL)
            {
                q->next = p->next;
            }
            else
            {
                table->buckets[pos] = p->next;
            }

            free(p->key);
            free(p);

            table->size--;

            return 1;
        }

        q = p;
        p = p->next;
      
    }

    // 未找到 key
    return 0;

}


// 查找
void* hash_get(HashTable* table, const char* key)
{
    size_t pos = hash_function(key, table->capacity);

    HashNode* p = table->buckets[pos];

    while (p != NULL)
    {
        // 如果 key 存在，返回 value
        if(strcmp(p->key, key) == 0)
        {
            return p->value;
        }

        p = p->next;
    }

    return NULL;
    
}

// 遍历哈希表
void hash_iterate(HashTable* table, void (*func)(const char*, void*))
{
    if (table == NULL || func == NULL) {
        return;
    }

    for (size_t i = 0; i < table->capacity; i++) {
        HashNode* current = table->buckets[i];
        while (current != NULL) {
            func(current->key, current->value);
            current = current->next;
        }
    }
}