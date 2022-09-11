/* ��������� ������������� #pragma once ������ �� ���,
����� ���������� �������� ���� ��� ����������
����������� ������ ���� ���*/
#pragma once
#include <string>
/*����������� ��� unsigned int ��� uint. ������ ����� � ����, ���
����� ����������� ����� uint, ���������� ����� ��������� unsigned int*/
typedef unsigned int uint;

std::string to_hex(uint value);
uint F1(uint X, uint Y, uint Z);
uint G1(uint X, uint Y, uint Z);
uint H1(uint X, uint Y, uint Z);
uint I1(uint X, uint Y, uint Z);
uint rotate_left(uint value, int shift);
std::string get_md5(std::string in);