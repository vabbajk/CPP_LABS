#pragma once
#include "QueueAlgorithms.h"

void printMenu();
char chooseTaskNtoM(char min, char max);

void handleAddElement(Queue<int>& q);
void handleShowQueue(Queue<int>& q);
void handleSortQueue(Queue<int>& q);
void handleFindElement(Queue<int>& q);
