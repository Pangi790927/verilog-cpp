#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <sstream>
#include <string>
#include <initializer_list>

#include <vector>

#include <GL/gl.h>
#include <GL/glu.h>

#ifndef DEBUG
    #ifdef DEBUG_PRINT
        #include <iostream>
        #define DEBUG(x) std::cout << __FUNCTION__ << ": " << x << std::endl;
    #else
        #define DEBUG(x) ;
    #endif
#endif

namespace Util {

	template <typename Type, int MAX_SIZE = 100>
	class StaticQueue {
	public:
		Type cont[MAX_SIZE];
		int start = 0;
		int end = 0;
		int size = 0;

		void insert (const Type& arg) {
			if (size < MAX_SIZE) {
				cont[start] = arg;
				start++;
				if (start >= MAX_SIZE)
					start = 0;
				size++;
			}
			else {
				cont[start] = arg;
				start++;
				end++;
				if (end >= MAX_SIZE)
					end = 0;
				if (start >= MAX_SIZE)
					start = 0;
			}
		}

		Type pop() {
			if (size > 0) {
				int toRet = end;
				size--;
				end++;
				if (end >= MAX_SIZE)
					end = 0;
				return cont[toRet];
			}
		}

		bool empty() {
			return size == 0;
		}
	};

    //Text not by const reference
    //so that the function can be used with a character array as argument
    template <typename Type>
    inline Type stringToNumber ( const std::string &Text )
    {
        std::stringstream ss(Text);
        Type result;
        return ss >> result ? result : 0;
    }

    template <typename Type>
    inline std::string numberToString ( Type Number )
    {
        std::stringstream ss;
        ss << Number;
        return ss.str();
    }

    template <typename Type, typename... Args>
    inline bool eq (Type a, Type b, Args... args) {
        return a == b && eq(args...);
    }

    template <typename Type>
    inline bool eq (Type a, Type b, Type c) {
        return a == b && b == c;
    }

    template <typename Type>
    inline bool eq (Type a, Type b) {
        return a == b;
    }

    template <typename Type>
    inline bool isEqualToAny (Type val, const std::initializer_list<Type>& list) {
        for (const auto& i : list) {
            if (val == i) {
                return true;
            }
        }
        return false;
    }

    static float PI = 3.141592653589;

    inline float toRadians (float angle) {
        return angle * PI / 180.0f;
    }

    inline std::string getOpenGLError() {
        GLenum errCode;
        const unsigned char *errString = NULL;
        
        if ((errCode = glGetError()) != GL_NO_ERROR) {
            errString = gluErrorString(errCode);
            return "OpenGL Error: " + std::string((const char*)errString) + " : " + Util::numberToString(errCode);
        }
        else {
            return "NO_ERROR";
        }
    }

    template <typename Out>
    inline void split(const std::string &s, char delim, Out result) {
        std::stringstream ss;
        ss.str(s);
        std::string item;
        
        while (getline(ss, item, delim)) {
            *(result++) = item;
        }

        if (ss.str() != "") {
            *(result++) = ss.str();   
        }
    }


	inline std::vector<std::string> split(const std::string &s, char delim) {
		std::vector<std::string> elems;
		split(s, delim, std::back_inserter(elems));
		return elems;
	}

	template< typename T >
	struct array_deleter
	{
		void operator ()( T const * p)
		{ 
			delete[] p; 
		}
	};

	template <typename A, typename B>
	struct same_class {
		const static bool value = false;
	};

	template <typename A>
	struct same_class<A, A> {
		const static bool value = true;
	};

	template <typename A, typename B, int value>
	struct if_true {
		using type = A;
	};

	template <typename A, typename B>
	struct if_true<A, B, false> {
		using type = B;
	};
}


#endif // UTIL_H_INCLUDED