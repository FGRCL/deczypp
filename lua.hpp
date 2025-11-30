#include <lua5.5/lua.hpp>
#include <string>
#include <variant>
#include <vector>

typedef std::variant<const char *, int> LuaKey;

class Selector {
private:
  LuaKey key;
  Selector *parent;

public:
  lua_State &luaState;

  Selector(LuaKey key, lua_State &luaState, Selector *parent);
  Selector operator[](LuaKey key);
  template <typename T> T get() {
    push();
    T result = convert(*this, static_cast<T *>(nullptr));
    pop();
    return result;
  }

protected:
  void push();
  void pop();
};

class LuaProgramResult {
private:
  lua_State &luaState;

public:
  LuaProgramResult(lua_State &luaState);
  Selector operator[](LuaKey key);
};

class LuaProgram {
private:
  lua_State &luaState;

public:
  LuaProgram();
  LuaProgramResult execute(std::string programFile);
};

template <typename T> T convert(Selector &selector, T *) {
  static_assert(false, "Lua converter is not implemented for type");
}

bool convert(Selector &selector, bool *);

double convert(Selector &selector, double *);

int convert(Selector &selector, int *);

std::string convert(Selector &selector, std::string *);

template <typename T>
std::vector<T> convert(Selector &selector, std::vector<T> *) {
  long n = lua_rawlen(&selector.luaState, -1);
  std::vector<T> result = std::vector<T>(n);
  for (int i = 1; i <= n; i++) {
    lua_pushinteger(&selector.luaState, i);
    lua_gettable(&selector.luaState, -2);
    T element = convert(selector, static_cast<T *>(nullptr));
    result[i - 1] = element;
    lua_pop(&selector.luaState, 1);
  }
  return result;
}
