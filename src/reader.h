#ifndef READER_H
#define READER_H

#include <vector>
#include <string>
#include "config.h"

void readInstance(const string& filename, int& num_pedidos, vector<Pedido>& pedidos, vector<vector<int>>& s);

#endif 