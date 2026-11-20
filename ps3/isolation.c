#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

#define BASE_HASH 0xCBF29CE484222325
#define HASH_MULTIPLIER 0x00000100000001B3

#define DIRECTION_LEFT 0b10101010
#define DIRECTION_RIGHT 0b01010101

struct Node {
    struct Node *right_child;
    int data;
    struct Node *left_child;
};

void establish_link(struct Node **target, int value);
void mutate_hash(uint64_t *hash, uint8_t modifier);
void process_structure(uint64_t *hash, const struct Node *element, int depth);
void attach_node(struct Node *current, int new_value);
void release_memory(struct Node *root);
struct Node *initialize_structure(int count, const int elements[count]);

int main()
{
    int total_structures, elements_per_structure;
    scanf("%d %d", &total_structures, &elements_per_structure);

    uint64_t unique_hashes[total_structures];
    int distinct_count = 0;

    int i = 0;
    while (i < total_structures)
    {
        unique_hashes[distinct_count] = BASE_HASH;
        int dataset[elements_per_structure];
        
        int j = 0;
        while (j < elements_per_structure)
        {
            scanf("%d", &dataset[j]);
            j++;
        }
        
        struct Node *structure = initialize_structure(elements_per_structure, dataset);
        process_structure(&unique_hashes[distinct_count], structure, 0);
        release_memory(structure);
        
        bool is_new_type = true;
        int k = distinct_count - 1;
        do
        {
            if (unique_hashes[distinct_count] == unique_hashes[k])
            {
                is_new_type = false;
            }
            k--;
        } while (k >= 0);
        
        if (is_new_type == true)
        {
            distinct_count++;
        }
        i++;
    }
    printf("%d\n", distinct_count);
}

void mutate_hash(uint64_t *hash, uint8_t modifier)
{
    *hash *= HASH_MULTIPLIER;
    *hash ^= modifier;
}

void process_structure(uint64_t *hash, const struct Node *element, int depth)
{
    if (element->right_child != NULL)
    {
        mutate_hash(hash, DIRECTION_RIGHT);
        mutate_hash(hash, depth);
        process_structure(hash, element->right_child, depth + 1);
    }
    
    if (element->left_child != NULL)
    {
        mutate_hash(hash, DIRECTION_LEFT);
        mutate_hash(hash, depth);
        process_structure(hash, element->left_child, depth + 1);
    }
}

struct Node *initialize_structure(int count, const int elements[count])
{
    struct Node *root = calloc(1, sizeof(struct Node));
    if (count > 0)
    {
        root->data = elements[0];
    }
    
    int i = 1;
    do
    {
        attach_node(root, elements[i]);
        i++;
    } while (i < count);
    
    return root;
}

void attach_node(struct Node *current, int new_value)
{
    if (new_value >= current->data)
    {
        establish_link(&current->right_child, new_value);
    }
    else
    {
        establish_link(&current->left_child, new_value);
    }
}

void establish_link(struct Node **target, int value)
{
    if (*target != NULL)
    {
        attach_node(*target, value);
    }
    else
    {
        *target = calloc(1, sizeof(struct Node));
        (*target)->data = value;
    }
}

void release_memory(struct Node *root)
{
    if (root->right_child != NULL)
    {
        release_memory(root->right_child);
    }
    
    if (root->left_child != NULL)
    {
        release_memory(root->left_child);
    }
    
    free(root);
}
