/*
 * Routes.c
 *
 *  Created on: Aug 19, 2023
 *      Author: Ronila
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Routes.h"

struct Route * initRoute(char* key, char* value)
{
	struct Route * root = (struct Route *) malloc(sizeof(struct Route));

	root->key = key;
	root->value = value;

	root->left = root->right = NULL;
	return root;
}

void inorder(struct Route* root)
{

    if (root != NULL)
    {
        inorder(root->left);
        printf("%s -> %s \n", root->key, root->value);
        inorder(root->right);
    }
}

void addRoute(struct Route ** root, char* key, char* value)
{
	if (*root == NULL) {
		 *root = initRoute(key, value);
	}
	else if (strcmp(key, (*root)->key) == 0)
	{
		printf("============ WARNING ============\n");
		printf("A Route For \"%s\" Already Exists\n", key);
	}
	else if (strcmp(key, (*root)->key) > 0)
	{
		addRoute(&(*root)->right, key, value);
	}
	else
	{
		addRoute(&(*root)->left, key, value);
	}
}

struct Route * search(struct Route * root, char* key)
{
	if (root == NULL)
	{
		return NULL;
	}

	if (strcmp(key, root->key) == 0){
		return root;
	}
	else if (strcmp(key, root->key) > 0)
	{
		return search(root->right, key);
	}
	else if (strcmp(key, root->key) < 0)
	{
		return search(root->left, key);
	}

	return NULL;

}


