#pragma once

#include <exception>
#include <string>

class Exception : public std::exception{
	protected:
		std::string message;
	public:
		Exception(const char* message) : message(message){}
		Exception(const std::string message) : message(message){}
		Exception() :message("[1;31mSomething went wrong[0m"){}
		virtual ~Exception() throw (){}
		virtual const char* what() const throw (){
	       return message.c_str();
	    }
};

class TooCloseError :      public Exception{};
class OutOfFieldError :    public Exception{};
class CantShootHereError : public Exception{};
