/*
 * Parameters.h
 *
 *  Created on: Aug 19, 2023
 *      Author: Ronila
 */

#ifndef PARAMETERS_H_
#define PARAMETERS_H_

// To Store key and value pairs
struct Parameter
{
	char* key;
	char* value;
};

// To store array of parameters
struct ParameterArray
{
	struct Parameter * parameters;
	bool is_sorted;
	size_t n_members;
	size_t capacity;
};

// Initialize array of parameters with capacity.
struct ParameterArray * paramInit(const size_t capacity); // Returns pointer to array on success, NULL on failure

// Parse parameters from a string (e.g., when receiving ROUTE url).
// Takes an existing Parameter pointer and adds new parameters parsed from string.
// This will MODIFY the url inplace to remove all the parameter text before it's passed
// on to render a static template.
void paramParse(struct ParameterArray * params, char* url);

// Clear all keys & values and set n_members back to zero.
void paramClear(struct ParameterArray * params); // This allows us to reuse the struct for multiple requests.

// Add new parameter to parameter array.
bool paramAdd(struct ParameterArray * params, const char* key, const char* value);

// Sort parameters by keys ascending. This is primarily used internally
void paramSort(struct ParameterArray * params);

// Return a pointer to a parameter with a given key.
// Any modifications done will be reflected in parameter array.
struct Parameter * paramGet(struct ParameterArray * params, const char* key);

// Free all memory consumed by parameters
void paramFree(struct ParameterArray * params);

#endif /* PARAMETERS_H_ */
