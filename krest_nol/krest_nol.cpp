#include <iostream>
//#include "stdafx.h"
#include <ctime>
#include <cstdlib>
#include <vector>
#include <thread>
#include <mutex>
using namespace std;
//Узел дерева
struct Uzel {
	//int revard = 0;
	int unic_number = 0;
	int level = 0;
	int **data = new int*[3];
	Uzel *next[9];
};
//Создание поля
int** inicialize_field(int size) {
	int **a = new int*[size];
	for (int i = 0; i < size; i++) {
		a[i] = new int[size];
		for (int j = 0; j < size; j++) {
			a[i][j] = 0;
		}
	}
	return a;
}
//Удаление поля
void _delete_(int** field, int size) {
	for (int j = 0; j < size; j++)
	{		
		delete[] field[j];
	}
	delete[] field;
}
void _delete_(int*** field,int size, int count_zeros) {
	for (int j = 0; j < count_zeros; j++)
	{
		for (int i = 0; i < size; i++) {
			delete[] field[j][i];
		}
		delete[] field[j];
	}	
	delete [] field;
}
//Удаление массива
template <typename T>
void _delete_(T* field) {	
	if (field != NULL) {
		delete[] field;
	}
}
//Удаление дерева
void _delete_(Uzel* field,int &count_dels) {	
	for (int i = 0; i < 9; i++)
	{
		if (field->next[i] != NULL) {			
			_delete_(field->next[i], count_dels);
		}		
	}	
	count_dels++;
	delete[] field;
}
//Вывод массива в строку
void print(int* arr,int size) {
	if (arr != NULL) {
		for (int i = 0; i < size; i++)
		{
			cout << arr[i] << " ";
		}
		cout << "\n";
	}
	else {
		cout << "Невозможно вывести пустой массив" << endl;
	}
}
//Вывод поля
void print(int **field,int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (field[i][j] > -1) {
				cout << " " << field[i][j] << " ";
			}
			else {
				cout << field[i][j] << " ";
			}
		}
		cout << endl;

	}
	cout << endl;
}
//Вывод нескольких полей в сртоку
void print_v2(int ***arr_field, int count, int size) {
	for (int i = 0; i < size; i++) {
		for (int next = 0; next < count; next++) {
			for (int j = 0; j < size; j++) {
				if (arr_field[next][i][j] > -1) {
					cout << " " << arr_field[next][i][j] << " ";
				}
				else {
					cout << arr_field[next][i][j] << " ";
				}
			}
			cout << "  ";
		}
		cout << endl;

	}
	cout << endl;
}
//Создание нового узла дерева
Uzel* Create_new_uzel() {
	Uzel *new_node = new Uzel;
	new_node->data = inicialize_field(3);
	for (int i = 0; i < 9; i++) {
		new_node->next[i] = NULL;
	}
	return new_node;
}
//Преобразование хода в координаты
int* convert_to_x_y(int num, int size) {
	int* xy = new int[2];
	int x = 0; int y = 0;
	//x - значение по вертикали		
	//y - значение по горизонтали
	//  x 0 1 2 
	// y
	// 0  0 0 0
	// 1  0 0 0
	// 2  0 0 0
	y = int(num / size);
	x = num - y * size;
	xy[0] = x;
	xy[1] = y;
	return xy;
}
//Преобразование координат в ход
int convert_x_y_to_step(int y, int x,int size) {
	int step;
	step = ( y * size ) + x;
	return step;
}
//Воод хода для игрока
void input_step(int **field, int size, int player) {
	int step;
	int* xy = new int[2];
	while (true) {
		cout << "Введите число от 1 до 9:" << endl;
		cin >> step;
		if ((step > 0) && (step < 10)) {
			step--;
			xy = convert_to_x_y(step, size);
			if (field[xy[1]][xy[0]] == 0) {
				cout << "Ход:" << step << endl;
				cout << "Координаты x: " << xy[0] << " y: " << xy[1] << endl;
				field[xy[1]][xy[0]] = player;
				print(field, size);
				return;
			}
			else {
				cout << "Ошибка ввода" << endl;
			}

		}
	}
}
//Воод хода для ПК
void input_step(int step,int **field, int size, int player) {	
	int* xy = new int[2];							
	xy = convert_to_x_y(step, size);
	if (field[xy[1]][xy[0]] == 0) {
		/*
		cout << "Ход:" << step << endl;
		cout << "Координаты x: " << xy[0] << " y: " << xy[1] << endl;
		*/
		field[xy[1]][xy[0]] = player;
		//print(field, size);
		return;
	}
	else {
		cout << "Ошибка ввода" << endl;
	}
}
//Ввод ходя для дерева
int input_auto(int **field, int size, int count, int player) {
	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			if (field[y][x] == 0) {
				if (count == 0) {
					field[y][x] = player;
					return convert_x_y_to_step(y,x,size);
				}
				else {
					count--;
				}
			}
		}
	}
}
//Копирование игрового поля в новое поле
void copy_arr(int **field_perent, int **field_child, int size) {
	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			field_child[y][x] = field_perent[y][x];
		}
	}
	return;
}
//Логика победы оптимизированная
bool logic(int** field, int size, int player) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (field[i][j] == player) {
				//Победа по вертикальному столбцу
				if ((field[0][j] == player) && (field[1][j] == player) && (field[2][j] == player)) { return true; }
				//Победа по горизонтальному столбцу
				if ((field[i][0] == player) && (field[i][1] == player) && (field[i][2] == player)) { return true; }
			}
			if (abs(i - j) != 1) {
				//Победа по диагонали
				if ((field[0][0] == player) && (field[1][1] == player) && (field[2][2] == player)) { return true; }
				if ((field[0][2] == player) && (field[1][1] == player) && (field[2][0] == player)) { return true; }
			}
		}
	}
	return false;
}
//Проверка на ничью
bool logic(int** field, int size) {
	int count_zeros = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (field[i][j] == 0) {
				return false;
			}
		}
	}
	return true;
}
//Логика победы не оптимизированная
bool logic_not_optimize(int** field, int size, int player) {
	for (int i = 0; i < size; i++) {
		//Победа по вертикальному столбцу
		if ((field[i][0] == player) && (field[i][1] == player) && (field[i][2] == player)) { return true; }
		//Победа по горизонтальному столбцу
		if ((field[0][i] == player) && (field[1][i] == player) && (field[2][i] == player)) { return true; }
		//Победа по диагонали
		if ((field[0][0] == player) && (field[1][1] == player) && (field[2][2] == player)) { return true; }
		if ((field[0][2] == player) && (field[1][1] == player) && (field[2][0] == player)) { return true; }
	}
	return false;
}
//Прогнозирование награды
void revard(int** field) {
	return;
}
//Рандомный ход противника
void random_step(int **field, int size, int player) {
	int* xy;
	int steps[9];
	for (int i = 0; i < 9; i++) {
		steps[i] = 0;
	}
	int cur_step = 0;
	for (int i = 0; i < 9; i++) {
		xy = convert_to_x_y(i, size);
		if (field[xy[1]][xy[0]] == 0) {
			steps[cur_step] = i;
			cur_step++;
		}
	}	
	int random = rand() % cur_step;	
	/*
	for (int i = 0; i < cur_step; i++) {
		cout << steps[i] << endl;
	}
	*/
	xy = convert_to_x_y(steps[random], size);
	field[xy[1]][xy[0]] = player;
	cout << "Противник сделал ход:"<< steps[random] << endl;
	print(field, size);
	return;
}
//Проход одного варианта по дереву
void check_variation(Uzel* root) 
{
	cout << "Поле одного прохода" << endl;
	Uzel* elem = root;
	for (int i = 0; i < 5; i++)
	{		
		print(elem->data, 3);
		cout << "Уникальный номер:" << elem->unic_number << endl;
		cout << "Глубина:" << elem->level << endl;
		if (elem->next[0] != NULL) {
			elem = elem->next[0];
		}
		else { break; }
	}
}
//Шифорвание матриц
/*
отрицательные числа: "-"
количество нулей <- 8; 
1 <- 1
положительные числа: "+"
1  -> 8
-1 -> 9
количество нулей -> 1-7
0  1 -1
-1 0  0 -> 189282
1  0  0

0  0  0
0  1  0 -> 48298
0 -1  1

0  0  0
0  0  1 -> 58198
0 -1  1

0  0  0
0  0  0 -> -81
0  0  1

0  0  0
0  0  0 -> -89
0  0  -1

1  0  0
0  0  0 -> -18
0  0  0

-1  0  0
0  0  0 -> -98
0  0  0
*/
//Шифрвание матрицы
int coding_matrix(int** field)
{
	int zeros_count = 0;
	int* xy;
	int size = 3;
	int coding_number = 0;
	for (int i = 0; i < 9; i++)
	{
		xy = convert_to_x_y(i,size);
		if (field[xy[1]][xy[0]] == 0) {
			zeros_count++;
			//Уникальный случай отрицательных номеров
			if (zeros_count == 8) {
				if (field[0][0] == 1) {
					coding_number = -18;
					return coding_number;
				}
				if (field[2][2] == 1)
				{
					coding_number = -81;
					return coding_number;
				}
				if (field[0][0] == -1) {
					coding_number = -98;
					return coding_number;
				}
				if (field[2][2] == -1) {
					coding_number = -89;
					return coding_number;
				}
			}
		}
		else {			
			//Стандартная шифровка			
			if (field[xy[1]][xy[0]] == 1) {
				if (zeros_count != 0) {
					coding_number = (coding_number * 10 + zeros_count) * 10 + 8;
				}
				else {
					coding_number = coding_number * 10 + 8;
				}
			}
			if (field[xy[1]][xy[0]] == -1) {
				if (zeros_count != 0) {
					coding_number = (coding_number * 10 + zeros_count) * 10 + 9;
				}
				else {
					coding_number = coding_number * 10 + 9;
				}
			}
			zeros_count = 0;
		}
	}
	if (zeros_count != 0) { coding_number = coding_number * 10 + zeros_count; }
	return coding_number;
}
//Поиск максимального элемента
int max(int* array, int size) {
	int max = 0;
	for (int i = 0; i < (sizeof(array)/sizeof(int)); i++)
	{
		if (max < array[i]) {
			max = array[i];
		}
	}
	return max;
}
int mean(int* array, int size) {
	int mean = 0;
	for (int i = 0; i < size; i++)
	{
		mean = mean + array[i];
	}
	return (mean/size);
}
//Поиск индекса максимального элемента
int index_max(int* array,int size) {
	int max = 0;
	int index_max = 0;
	for (int i = 0; i < size; i++)
	{
		if (max < array[i]) {
			max = array[i];
			index_max = i;
		}
	}
	return index_max;
}
//Не пригодилось
void delet_vetv(){}
void delet_dublicate(Uzel* root, int level, vector<Uzel*> *arr_fields)
{
	for (int i = 0; i < 9; i++)
	{
		/*
		if (root->next[i] != NULL) {
			if (root->next[i]->level == level) {
				(*arr_fields).push_back(root->next[i]);
				if (root->next[i] == ссылка на повторяющийся объект) {
					delet_vetv(root->next[i]);
				}
				//cout << "Найден элемент подходящего уровня" << endl;
			}
			if (root->next[i]->level < level) {
				check_level(root->next[i], level, arr_fields);
			}
		}
		*/
	}
	return;
}
void check_level (Uzel* root,int level,vector<Uzel*> *arr_fields)
{
	for (int i = 0; i < 9; i++)
	{
		if (root->next[i] != NULL) {
			if (root->next[i]->level == level) {
				(*arr_fields).push_back(root->next[i]);
			}
			if (root->next[i]->level < level) {
				check_level(root->next[i], level, arr_fields);
			}
		}		
	}
	return;
}
void duplicate_search(Uzel* root) {
	Uzel* elem = root;
	for (int level = 3; level <= 4; level++)
	{		
		vector <Uzel*> arr_fields;
		check_level(root,level,&arr_fields);
		cout << "Количество элементов на " << level << " уровне :" << arr_fields.size() << endl;
		if (arr_fields.size()>0) {
			cout << "Ссылка на этот элемент: " << arr_fields[0] << endl;
			cout << "Первый элемент" << endl;
			print(arr_fields[0]->data,3);
			cout << "Его уникальный номер" << endl;
			cout << arr_fields[0]->unic_number << endl;
		}

		int count_dublicate = 0;
		for (int i = 0; i < arr_fields.size(); i++)
		{			
			for (int j = i+1; j < arr_fields.size(); j++)
			{				
				if (arr_fields[i]->unic_number == arr_fields[j]->unic_number) {
					arr_fields[j] = arr_fields[i];
					count_dublicate++;
				}
			}
		}
		cout << "Количество совпадающих полей: " << count_dublicate << endl;
	}
}
//Инициализация параметров
void inicialization(int* parametrs) {	
	for (int i = 0; i < 6; i++)
	{
		parametrs[i] = rand() % 100;
	}
	return;
}
//Расчёт награды
int revard_calculate(int** field,int size,int player, int *parametrs) {
	int rev=0;	
	int count_1 = 0;
	for (int i = 0; i < size; i++) {
		count_1=0;
		//Ловушка по вертикальному столбцу
		for (int j = 0; j < 3; j++)
		{
			if (field[i][j] == player) { count_1++; }
		}		
		if (count_1 == 2) { rev += parametrs[2]; }
		count_1 = 0;
		//Ловушка по горизонтальному столбцу
		for (int j = 0; j < 3; j++)
		{
			if (field[j][i] == player) { count_1++; }
		}
		if (count_1 == 2) { rev += parametrs[3]; }
		count_1 = 0;				
	}	
	//Ловушка по диагонали
	for (int i = 0; i < 3; i++)
	{
		if (field[i][i] == player) { count_1++; }
	}	
	if (count_1 == 2) { rev += parametrs[4]; }
	count_1 = 0;
	for (int i = 0; i < 3; i++)
	{
		if (field[i][2-i] == player) { count_1++; }
	}
	if (count_1 == 2) { rev+= parametrs[4]; }
	count_1 = 0;
	//Ход в центр
	if (field[1][1] == player){rev += parametrs[5];}
	return rev;
}
//Автоматический ход недопускающий победу в место вариативности
int zapolnenie(int** field, int size, int player) {		
	int count_zeros = 0;
	int** cur_field = new int*[3];
	for (int i = 0; i < size; i++)
	{
		cur_field[i] = new int[3];
	}
	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {			
			if (field[y][x] == 0) {
				copy_arr(field, cur_field, size);
				count_zeros++;
				cur_field[y][x] = player * (-1);
				if (logic(cur_field, size, player*(-1))) {
					_delete_(cur_field, size);
					return convert_x_y_to_step(y, x, size);
				}
			}
		}
	}	
	_delete_(cur_field, size);
	return -1;
}
//Обновление дерева
Uzel* Update_tree(Uzel* cur_root) {
	int count = 0;
	if (cur_root != NULL) {
		cout << "Есть нестёртый массив на входе" << endl;
		for (int i = 0; i < 9; i++)
		{
			if (cur_root->next[i] != NULL) {
				int x;
				_delete_(cur_root, count);
				cout << "Вызвалось удаление старого дерева" << endl;				
				cur_root = Create_new_uzel();
				break;
			}
		}		
	}
	cout << "Количество удалённых узлов:" << count << endl;
	return cur_root;
}
//Подсчёт количества узлов в дереве
int Calculate_range_tree(Uzel* field) {
	int loc_count = 1;
	if (field != NULL) {
		for (int i = 0; i < 9; i++)
		{
			if (field->next[i] != NULL) {
				loc_count = loc_count + Calculate_range_tree(field->next[i]);
			}
		}
	}
	return loc_count;
}
//-------------------Дерево-------------------
//Рекурсивное дерево
int tree(int** field, Uzel *cur_root, int size, int &count_wins, int current_level, int player, int deep, int revard, int *parametrs) {
	if ((deep == 0) || (deep > 9)) { return 0; }
	if (current_level == 0) {
		return revard;
	}
	else {
		//Поиск количества ходов
		int count_zeros = 0;
		for (int y = 0; y < 3; y++) {
			for (int x = 0; x < 3; x++) {
				if (field[y][x] == 0)
					count_zeros++;
			}
		}
		if (count_zeros == 0) {
			return 0;
		}			
		//Проверка на автоматическое заполнение поля		
		if (count_zeros <= 4) {			
			int auto_step = zapolnenie(field, size, player);
			if (auto_step != -1) {
				int loc_revard;							
				int ** zap_field = inicialize_field(size);
				copy_arr(field, zap_field, size);
				/*
				cout << "===================" << endl;
				print(field, size);
				cout << "Автоход: " << auto_step << endl;				
				*/
				input_step(auto_step,zap_field,size,player);
				cur_root->next[0] = Create_new_uzel();
				cur_root->next[0]->level = current_level;
				copy_arr(zap_field, cur_root->next[0]->data, size);
				loc_revard = (revard_calculate(zap_field, size, player*pow(-1, current_level + 1),parametrs) - revard_calculate(zap_field, size, player*pow(-1, current_level),parametrs));
				loc_revard = tree(zap_field, cur_root->next[0], size, count_wins, current_level + 1, player*(-1), deep - 1, loc_revard, parametrs);
				_delete_(zap_field,size);	
				return revard + loc_revard;
			}
		}		
		//Создание ветвей дерева
		int ***arr = new int**[count_zeros];
		for (int i = 0; i < count_zeros; i++) {
			arr[i] = inicialize_field(size);
			copy_arr(field, arr[i], size);
		}
		//Совершение ходов
		bool *win1 = new bool[count_zeros];
		bool *win2 = new bool[count_zeros];				
		int *arr_revard = new int[count_zeros];
		for (int i = 0; i < count_zeros; i++) {
			input_auto(arr[i], size, i, player);
			cur_root->next[i] = Create_new_uzel();
			cur_root->next[i]->level = current_level;
			copy_arr(arr[i], cur_root->next[i]->data, size);
			//cur_root->next[i]->unic_number = coding_matrix(cur_root->next[i]->data);			
			win1[i] = logic(arr[i], size, player*pow(-1, current_level + 1));
			win2[i] = logic(arr[i], size, player*pow(-1, current_level));
			if (win1[i]) {
				arr_revard[i] = parametrs[0]; // Награда дочернего элемента за победу								
				count_wins++;	
				//print(arr[i],size);
			}
			if (win2[i]) {
				arr_revard[i] = parametrs[1]; // Награда дочернего элемента за проигрыш								
				count_wins++;				
				//print(arr[i], size);
			}
		}		
		//Проверка ветвей на победу					
		for (int i = 0; i < count_zeros; i++) {
			if ((win1[i] == false) && (win2[i] == false)) {		//Случай если победитель ещё не найден	
				arr_revard[i] = (revard_calculate(arr[i], size, player*pow(-1, current_level + 1), parametrs) - revard_calculate(arr[i], size, player*pow(-1, current_level), parametrs));
				arr_revard[i] = tree(arr[i], cur_root->next[i], size, count_wins, current_level + 1, player*(-1), deep - 1, arr_revard[i], parametrs);
			}
		}		
		
		int max_rev = mean(arr_revard, count_zeros); //count zeros = 0!
		_delete_(win1);
		_delete_(win2);
		_delete_(arr_revard);
		_delete_(arr, size, count_zeros);
		//Вернуть максимальную награду от дочерних элементов и текущего элемента			
		return revard + max_rev;
	}
}
//Корень дерева
int tree(int** field, Uzel *cur_root, int size, int &count_wins, int current_level, int player, int deep, int *parametrs) {
	if ((deep == 0) || (deep > 9)) { return 0; }
	if (current_level == 0) {
		//print(field, size);		
		cur_root = Update_tree(cur_root);
		cur_root = Create_new_uzel();
		copy_arr(field, cur_root->data, size);
		int res = tree(field, cur_root, size, count_wins, current_level + 1, player, deep, parametrs);
		cout << "Выход из корневого дерева: " << res << endl;
		return res;
	}
	else {
		//Поиск количества ходов
		int count_zeros = 0;
		for (int y = 0; y < 3; y++) {
			for (int x = 0; x < 3; x++) {
				if (field[y][x] == 0)
					count_zeros++;
			}
		}
		//Проверка на автоматическое заполнение поля	
		if (count_zeros <= 4) {
			int auto_step = zapolnenie(field, size, player);
			if (auto_step != -1) {				
				return auto_step;
			}
		}
		//Создание ветвей дерева
		int ***arr = new int**[count_zeros];
		for (int i = 0; i < count_zeros; i++) {
			arr[i] = inicialize_field(size);
			copy_arr(field, arr[i], size);
		}
		//Совершение ходов
		int* arr_steps = new int[count_zeros];
		bool *win1 = new bool[count_zeros];
		bool *win2 = new bool[count_zeros];
		int *arr_revard = new int[count_zeros];
		for (int i = 0; i < count_zeros; i++) {
			arr_steps[i] = input_auto(arr[i], size, i, player);
			cur_root->next[i] = Create_new_uzel();
			cur_root->next[i]->level = current_level;		
			copy_arr(arr[i], cur_root->next[i]->data, size);
			win1[i] = logic(arr[i], size, player*pow(-1, current_level + 1));
			win2[i] = logic(arr[i], size, player*pow(-1, current_level));
			if (win1[i]) {
				arr_revard[i] = parametrs[0]; // Награда дочернего элемента за победу								
				count_wins++;				
			}
			if (win2[i]) {
				arr_revard[i] = parametrs[1]; // Награда дочернего элемента за проигрыш								
				count_wins++;				
			}
		}
		//Проверка на необходимость продолжать дерево		
		
		for (int i = 0; i < count_zeros; i++) {			
			arr_revard[i] = (revard_calculate(arr[i], size, player*pow(-1, current_level + 1), parametrs) - revard_calculate(arr[i], size, player*pow(-1, current_level), parametrs));
			arr_revard[i] = tree(arr[i], cur_root->next[i], size, count_wins, current_level + 1, player*(-1), deep - 1, arr_revard[i], parametrs);
		}
		//Multiprocessing wrong
		/*
		thread t1([&]() {if (count_zeros >= 1) { cout << "Запустилось 1 ядро" << endl; arr_revard[0] = tree(arr[0], cur_root->next[0], size, count_wins, current_level + 1, player*(-1), deep - 1, arr_revard[0]); }});
		thread t2([&]() {if (count_zeros >= 2) { cout << "Запустилось 2 ядро" << endl;arr_revard[1] = tree(arr[1], cur_root->next[1], size, count_wins, current_level + 1, player*(-1), deep - 1, arr_revard[1]); }});
		thread t3([&]() {if (count_zeros >= 3) { cout << "Запустилось 3 ядро" << endl;arr_revard[2] = tree(arr[2], cur_root->next[2], size, count_wins, current_level + 1, player*(-1), deep - 1, arr_revard[2]); }});
		thread t4([&]() {if (count_zeros >= 4) { cout << "Запустилось 4 ядро" << endl;arr_revard[3] = tree(arr[3], cur_root->next[3], size, count_wins, current_level + 1, player*(-1), deep - 1, arr_revard[3]); }});
		thread t5([&]() {if (count_zeros >= 5) { cout << "Запустилось 5 ядро" << endl;arr_revard[4] = tree(arr[4], cur_root->next[4], size, count_wins, current_level + 1, player*(-1), deep - 1, arr_revard[4]); }});
		thread t6([&]() {if (count_zeros >= 6) { cout << "Запустилось 6 ядро" << endl;arr_revard[5] = tree(arr[5], cur_root->next[5], size, count_wins, current_level + 1, player*(-1), deep - 1, arr_revard[5]); }});
		thread t7([&]() {if (count_zeros >= 7) { cout << "Запустилось 7 ядро" << endl;arr_revard[6] = tree(arr[6], cur_root->next[6], size, count_wins, current_level + 1, player*(-1), deep - 1, arr_revard[6]); }});
		thread t8([&]() {if (count_zeros >= 8) { cout << "Запустилось 8 ядро" << endl;arr_revard[7] = tree(arr[7], cur_root->next[7], size, count_wins, current_level + 1, player*(-1), deep - 1, arr_revard[7]); }});
		thread t9([&]() {if (count_zeros >= 9) { cout << "Запустилось 9 ядро" << endl;arr_revard[8] = tree(arr[8], cur_root->next[8], size, count_wins, current_level + 1, player*(-1), deep - 1, arr_revard[8]); }});
		
		t1.join();
		t2.join();
		t3.join();
		t4.join();
		t5.join();
		t6.join();
		t7.join();
		t8.join();
		t9.join();
		*/
		int max_index = arr_steps[index_max(arr_revard, count_zeros)];
		cout << "Незавершающий цикл:" << max_index << endl;
		print(arr_steps, count_zeros);
		print(arr_revard, count_zeros);
		cout << "Количество возможных ходов: " << count_zeros << endl;

		_delete_(win1);
		_delete_(win2);
		_delete_(arr, size, count_zeros);
		_delete_(arr_steps);
		_delete_(arr_revard);
		return max_index;
	}
}
//Рандомизатор параметров
int* random() {
	int *param = new int[6];
	param[0] = rand() % 100;
	param[1] = rand() % 100 - 30;
	param[2] = rand() % 100;
	param[3] = rand() % 100; 
	param[4] = rand() % 100;
	param[5] = rand() % 100;	
	return param;
}
//Поиск более эффективаных параметров
int find_more_effective_player(int** field, int size,int player){	
	int count_1 = 0;
	int rev = 0;
	for (int i = 0; i < size; i++) {
		count_1 = 0;
		//Ловушка по вертикальному столбцу
		for (int j = 0; j < 3; j++)
		{
			if (field[i][j] == player) { count_1++; }
		}
		if (count_1 == 2) { rev++; }
		count_1 = 0;
		//Ловушка по горизонтальному столбцу
		for (int j = 0; j < 3; j++)
		{
			if (field[j][i] == player) { count_1++; }
		}
		if (count_1 == 2) { rev++; }
		count_1 = 0;
	}
	//Ловушка по диагонали
	for (int i = 0; i < 3; i++)
	{
		if (field[i][i] == player) { count_1++; }
	}
	if (count_1 == 2) { rev ++; }
	count_1 = 0;
	for (int i = 0; i < 3; i++)
	{
		if (field[i][2 - i] == player) { count_1++; }
	}
	if (count_1 == 2) { rev++; }
	count_1 = 0;
	//Ход в центр
	if (field[1][1] == player) { rev ++; }
	return rev;
}
int main()
{		
	srand(static_cast<unsigned int>(time(0)));
	vector<Uzel> b;
	Uzel *root = NULL;
	Uzel *root1 = NULL;
	setlocale(LC_ALL, "ru");
	int size = 3;
	int win_0=0;
	bool win = false, nichya = false;
	int **a = inicialize_field(size);	
	//parametrs = {win, lose, trap_vert, trap_gorizont, trap_dia, central_step}
	int parametrs[6] = {10, -2, 1, 1, 2, 2};
	int  player = 1;
	while (!win) {
		char _if_;	
		win_0 = 0;
		cout << "Хотите сделать ход?\n1 - Сделать ход\n2 - Построить дерево\n3 - Сыграть между двумя деревьями\n4 - Выход" << endl;
		cin >> _if_;
		switch (_if_)
		{
		case '1': {			
			if ((win == false) && (nichya == false)) {
				input_step(a, size, player);
				win = logic(a, size, player);
				nichya = logic(a, size);
				player = player * (-1);
				if ((win == false)&&(nichya==false)) {	
					root = Update_tree(root);
					root = Create_new_uzel();
					int predict = tree(a, root, size, win_0, 0, player, 7, parametrs); //Новое дерево
					input_step(predict, a, size, player);
					print(a,size);
					win = logic(a, size, player);
					player = player * (-1);
				}
			}
			if (win == true) { cout << "Game Over!" << endl; }
			if (nichya == true) { cout << "Nichya!" << endl; }
			break; 
		}
		case '2': {
			int predict;						
			int start_time = clock();
			//cout << Calculate_range_tree(root) << endl;
			//root = Update_tree(root);
			//root = Create_new_uzel();
			predict = tree(a, root, size,win_0,0, player, 3, parametrs);	//новое дерево	
			//check_variation(root);
			int end_time = clock();
			int search_time = end_time - start_time;
			cout << "Лучший ход: " << predict << endl;
			cout << "Количество побед: " << win_0 << endl;
			cout << "Время построения дерева:" << search_time << endl;
			break; }
		case '3': {		
			int * param1 = random();
			int * param2 = random();
			int winer = 0;
			cout << "Параметры первого:" << endl;
			cout << "{win, lose, trap_vert, trap_gorizont, trap_dia, central_step}" << endl;
			print(param1, 6);
			cout << "Параметры второго:" << endl;
			cout << "{win, lose, trap_vert, trap_gorizont, trap_dia, central_step}" << endl;
			print(param2,6);
			bool flag = true;
			while (flag)
			{
				if ((win == false) && (nichya == false)) {
					player = 1;
					cout << "Player: " << player << endl;
					int predict = tree(a, root, size, win_0, 0, player, 5, param1);
					input_step(predict, a, size, player);
					print(a, size);
					win = logic(a, size, player);
					nichya = logic(a, size);
					player *= (-1);
					if ((win == false) && (nichya == false)) {
						cout << "Player: " << player << endl;
						int predict1 = tree(a, root1, size, win_0, 0, player, 5, param2);
						input_step(predict1, a, size, player);
						print(a, size);
						win = logic(a, size, player);
						nichya = logic(a, size);
					}
				}
				if (win == true) { 
					cout << "Game Over!" << endl; 
					flag = false; 
					if (logic(a, size, 1)) 
					{ 
						winer = 1; 
						cout << "Победил первый" << endl; 
					} 
					else 
					{ 
						winer = -1;
						cout << "Победил второй" << endl; 
					} 
				}
				if (nichya == true) { 
					cout << "Nichya!" << endl; 
					flag = false; 
					if (find_more_effective_player(a, size, 1) - find_more_effective_player(a, size, -1) > 0) 
					{ 
						winer = 1;
						cout << "Победил первый" << endl; 
					} 
					else 
					{ 
						winer = -1; 
						cout << "Победил второй" << endl;
					} 
				}
				
			}
			if (winer = 1) {
				param2 = random();
			}
			else {
				param1 = random();
			}
			break;
		}
		case '4': _delete_(a, size); return 0;
		default:
			break;
		}
	}
}