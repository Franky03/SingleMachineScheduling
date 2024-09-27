#ifndef READER_H
#define READER_H

#include <vector>
#include <string>
#include "config.h"

void readInstance(const string& filename, int& num_pedidos, vector<Pedido>& pedidos, unordered_map<pair<int, int>, int, hash_pair>& s);

#endif 