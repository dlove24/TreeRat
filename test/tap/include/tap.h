#include <iostream>
#include <string>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/is_floating_point.hpp>
#include <boost/utility/enable_if.hpp>
#include <cmath>

namespace TAP {
namespace details {
struct skip_all_type {};
struct no_plan_type {};
extern std::ostream* output;
extern std::ostream* error;
}
class fatal_exception : public std::exception {
    std::string message;
  public:
    fatal_exception (const std::string& _message) : message (_message) {
      }
    const char* what() const throw() {
      return message.c_str();
      }
    ~fatal_exception() throw() {
      }
  };
extern const details::skip_all_type skip_all;
extern const details::no_plan_type no_plan;
void plan (unsigned) throw (fatal_exception);
void plan (const details::skip_all_type&, const std::string& = "") throw (fatal_exception);
void plan (const details::no_plan_type&) throw();
void done_testing() throw (fatal_exception);
void done_testing (unsigned) throw (fatal_exception);

unsigned planned() throw();
unsigned encountered() throw();

bool ok (bool, const std::string& = "") throw();
bool not_ok (bool, const std::string& = "") throw();

bool pass (const std::string& = "") throw();
bool fail (const std::string& = "") throw();

void skip (unsigned, const std::string& = "") throw();
void bail_out (const std::string& reason) throw();

int exit_status() throw();
bool summary() throw();

void set_output (std::ostream&) throw (fatal_exception);
void set_error (std::ostream&) throw (fatal_exception);

template<typename T> bool diag (const T& first) throw() {
  *details::error << "# " << first << std::endl;
  return false;
  }
template<typename T1, typename T2> bool diag (const T1& first, const T2& second) throw() {
  *details::error << "# " << first << second << std::endl;
  return false;
  }
template<typename T1, typename T2, typename T3> bool diag (const T1& first, const T2& second, const T3& third) throw() {
  *details::error << "# " << first << second << third << std::endl;
  return false;
  }
template<typename T1, typename T2, typename T3, typename T4> bool diag (const T1& first, const T2& second, const T3& third, const T4& fourth) throw() {
  *details::error << "# " << first << second << third << fourth << std::endl;
  return false;
  }
template<typename T1, typename T2, typename T3, typename T4, typename T5> bool diag (const T1& first, const T2& second, const T3& third, const T4& fourth, const T5& fifth) throw() {
  *details::error << "# " << first << second << third << fourth << fifth << std::endl;
  return false;
  }

template<typename T> bool note (const T& first) throw() {
  *details::output << "# " << first << std::endl;
  return false;
  }
template<typename T1, typename T2> bool note (const T1& first, const T2& second) throw() {
  *details::output << "# " << first << second << std::endl;
  return false;
  }
template<typename T1, typename T2, typename T3> bool note (const T1& first, const T2& second, const T3& third) throw() {
  *details::output << "# " << first << second << third << std::endl;
  return false;
  }
template<typename T1, typename T2, typename T3, typename T4> bool note (const T1& first, const T2& second, const T3& third, const T4& fourth) throw() {
  *details::output << "# " << first << second << third << fourth << std::endl;
  return false;
  }
template<typename T1, typename T2, typename T3, typename T4, typename T5> bool note (const T1& first, const T2& second, const T3& third, const T4& fourth, const T5& fifth) throw() {
  *details::output << "# " << first << second << third << fourth << fifth << std::endl;
  return false;
  }

template<typename T, typename U> typename boost::disable_if<typename boost::is_floating_point<U>::type, bool>::type is (const T& left, const U& right, const std::string& message = "") {
  try {
    bool ret = ok (left == right, message);

    if (!ret) {
      note ("Failed test '", message, "'");
      note ("       Got: ", left);
      note ("  Expected: ", right);
      }

    return ret;
    }

  catch (const std::exception& e) {
    fail (message);
    note ("Failed test '", message, "'");
    note ("Cought exception '", e.what(), "'");
    note ("       Got: ", left);
    note ("  Expected: ", right);
    return false;
    }

  catch (...) {
    fail (message);
    note ("Failed test '", message, "'");
    note ("Cought unknown exception");
    note ("       Got: ", left);
    note ("  Expected: ", right);
    return false;
    }
  }

template<typename T, typename U> typename boost::disable_if<typename boost::is_floating_point<U>::type, bool>::type isnt (const T& left, const U& right, const std::string& message = "") {
  try {
    return ok (left != right, message);
    }

  catch (const std::exception& e) {
    fail (message);
    note ("In test ", message);
    note ("Cought exception: ", e.what());
    return false;
    }

  catch (...) {
    fail (message);
    note ("In test ", message);
    note ("Cought unknown exception");
    return false;
    }
  }

template<typename T, typename U> typename boost::enable_if<typename boost::is_floating_point<U>::type, bool>::type is (const T& left, const U& right, const std::string& message = "", double epsilon = 0.01) {
  try {
    bool ret = ok (2 * fabs (left - right) / (fabs (left) + fabs (right)) < epsilon);

    if (!ret) {
      note ("Failed test '", message, "'");
      note ("       Got: ", left);
      note ("  Expected: ", right);
      }

    return ret;
    }

  catch (const std::exception& e) {
    fail (message);
    note ("Failed test '", message, "'");
    note ("Cought exception '", e.what(), "'");
    note ("       Got: ", left);
    note ("  Expected: ", right);
    return false;
    }

  catch (...) {
    fail (message);
    note ("Failed test '", message, "'");
    note ("Cought unknown exception");
    note ("       Got: ", left);
    note ("  Expected: ", right);
    return false;
    }
  }

template<typename T, typename U> typename boost::enable_if<typename boost::is_floating_point<U>::type, bool>::type isnt (const T& left, const U& right, const std::string& message = "", double epsilon = 0.01) {
  try {
    bool ret = 2 * fabs (left - right) / (fabs (left) + fabs (right)) > epsilon;
    ok (ret, message);
    return ret;
    }

  catch (const std::exception& e) {
    fail (message);
    note ("Failed test '", message, "'");
    note ("Cought exception '", e.what(), "'");
    return false;
    }

  catch (...) {
    fail (message);
    note ("Failed test '", message, "'");
    note ("Cought unknown exception");
    return false;
    }
  }

template<typename T, typename U> bool is_convertible (const std::string& message) {
  return ok (boost::is_convertible<T, U>::value, message);
  }

template<typename T, typename U> bool is_inconvertible (const std::string& message) {
  return ok (!boost::is_convertible<T, U>::value, message);
  }

}

#ifdef WANT_TEST_EXTRAS

namespace TAP {
namespace details {
struct Skip_exception {
  const std::string reason;
  Skip_exception (const std::string& _reason) throw() : reason (_reason) {
    }
  };
struct Todo_exception {
  const std::string reason;
  Todo_exception (const std::string& _reason) throw() : reason (_reason) {
    }
  };

void start_block (unsigned) throw();
unsigned stop_block() throw (fatal_exception);

class todo_guard {
    const std::string value;
  public:
    todo_guard() throw();
    ~todo_guard() throw();
  };
}

void skip (const std::string& reason) throw (details::Skip_exception);
void skip_todo (const std::string& reason) throw (details::Todo_exception);
}

#define TRY(action, name) do {\
    try {\
      action;\
      TAP::pass(name);\
      }\
    catch (const std::exception& e) {\
      TAP::fail(name);\
      note("Caught exception: ", e.what());\
      }\
    catch (...) {\
      TAP::fail(name);\
      }\
    } while (0)

#define FAIL(action, name) do {\
    try {\
      action;\
      TAP::fail(name);\
      }\
    catch (...) {\
      TAP::pass(name);\
      }\
    } while (0)

#define TEST_START(num) {\
    const char* _current_message = NULL;\
    TAP::plan(num);\
    try {

#define TEST_END \
  if (TAP::encountered() < TAP::planned()) {\
    TAP::note("Looks like you planned ", TAP::planned(), " tests but only ran ", TAP::encountered());\
    }\
  else if(TAP::encountered() > TAP::planned()) {\
    TAP::note("Looks like you planned ", TAP::planned(), " tests but ran ", TAP::encountered() - TAP::planned(), " extra");\
    }\
    }\
  catch(TAP::details::Skip_exception& skipper) {\
    TAP::skip(TAP::planned() - TAP::encountered(), skipper.reason);\
    }\
  catch(TAP::details::Todo_exception& todoer) {\
    /*TODO*/\
    }\
  catch(const TAP::fatal_exception& e) {\
    if(_current_message) TAP::fail(_current_message);\
    note("A fatal error occured:,", e.what()," aborting");\
    }\
  catch(const std::exception& e) {\
    if(_current_message) TAP::fail(_current_message);\
    note("Got unknown error: ", e.what());\
    }\
  catch (...) {\
    if(_current_message) TAP::fail(_current_message);\
    }\
  return TAP::exit_status();\
    }

#define BLOCK_START(planned) \
  try {\
    details::todo_guard foo##planned;\
    TAP::details::start_block(planned);


#define BLOCK_END \
  if (TAP::encountered() != TAP::details::stop_block()) {\
    note("There seems to be a wrong number of tests!");\
    }\
    }\
  catch(TAP::details::Skip_exception& skipper) {\
    TAP::skip(TAP::details::stop_block() - TAP::encountered(), skipper.reason);\
    }\
  catch(TAP::details::Todo_exception& todoer) {\
    note("Can't handle todo properly yet");\
    /*TODO*/\
    }\
  catch(const std::exception& e) {\
    TAP::fail(_current_message);\
    note("Got error: ", e.what());\
    }\
  catch (...) {\
    TAP::fail(_current_message);\
    note("Died with some mysterious error");\
    }

/* This small macro is a main reason for this ugly exercise. I can't introduce a new scope because
 * code subsequent to the declaration should be visible to the rest of the code. At the same time, it
 * must be exception safe. They are quite severe constraints :-(.
 */
#define TRY_DECL(action, new_message) \
  _current_message = new_message;\
  action;\
  TAP::pass(_current_message);\
  _current_message = NULL

#endif /*WANT_TEST_EXTRAS*/
