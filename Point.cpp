#include "Point.hpp"

IntegerPoint::IntegerPoint ( ) {

	x = 0;
	y = 0;
	z = 0; }

IntegerPoint::IntegerPoint ( UINT64 x, UINT64 y, UINT64 z ) {

	this->x = x;
	this->y = y;
	this->z = z; }

RealPoint::RealPoint ( ) {

	x = 0.00;
	y = 0.00;
	z = 0.00; }

RealPoint::RealPoint ( REAL64 x, REAL64 y, REAL64 z ) {

	this->x = x;
	this->y = y;
	this->z = z; }