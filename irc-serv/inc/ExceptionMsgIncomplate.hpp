#ifndef EXCEPT_MSGINCOMPLATE_HPP
# define EXCEPT_MSGINCOMPLATE_HPP

# include <exception>
# include <string>
# include <typeinfo>

using std::exception;
using std::string;

class IRC_MsgIncomplate: public exception {
	public:
		IRC_MsgIncomplate() : msg_(typeid(*this).name()) {}

		/** Constructor (C strings).
		 *  @param message	The error message.
		 *  				the function is explicit to block implicitly cast
		 */
		explicit IRC_MsgIncomplate(const char* message) 
			: msg_(string(message)) {}
		explicit IRC_MsgIncomplate(const std::string& message)
			: msg_(string(message)) {}

		virtual ~IRC_MsgIncomplate() throw() {}
		virtual const char* what() const throw() {
			return msg_.c_str();
		}
	protected:
		std::string msg_;
};


#endif // EXCEPT_MSGINCOMPLATE_HPP
