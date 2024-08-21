/*
 * Routes.h
 *
 *  Created on: Aug 19, 2023
 *      Author: Ronila
 */

#ifndef ROUTES_H_
#define ROUTES_H_

struct Route {
	char* key;
	char* value;

	struct Route *left, *right;
};

struct Route * initRoute(char *key, char *value);

void addRoute(struct Route **root, char *key, char *value);

struct Route * search(struct Route *root, char *key);

void inorder(struct Route *root );


#endif /* ROUTES_H_ */
