#include "lua.hpp"
#include <lua5.5/lua.hpp>
#include <string>
#include <variant>

// TODO namespace

template <class... Ts> struct overloaded : Ts... {
  using Ts::operator()...;
};
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

Selector::Selector(LuaKey key, lua_State &luaState, Selector *parent)
    : key(key), parent(parent), luaState(luaState) {}

Selector Selector::operator[](LuaKey key) {
  return Selector(key, luaState, this);
};

void Selector::push() {
  if (parent == nullptr) {
    std::visit(
        overloaded{
            [](int key) {}, // TODO error handling or add selector stragegy
            [this](const char *key) { lua_getglobal(&luaState, key); }},
        key);
  } else {
    parent->push();
    std::visit(
        overloaded{[this](int key) { lua_pushinteger(&luaState, key); },
                   [this](const char *key) { lua_pushstring(&luaState, key); }},
        key);
    lua_gettable(&luaState, -2);
  }
}

void Selector::pop() {
  lua_pop(&luaState, 1);
  if (parent != nullptr) {
    parent->pop();
  }
};

LuaProgramResult::LuaProgramResult(lua_State &luaState) : luaState(luaState) {};

Selector LuaProgramResult::operator[](LuaKey key) {
  return Selector(key, luaState, nullptr);
};

LuaProgram::LuaProgram() : luaState(*luaL_newstate()) {}

LuaProgramResult LuaProgram::execute(std::string programFile) {
  lua_rawlen(&luaState, -1);
  if (luaL_loadfile(&luaState, programFile.c_str()) ||
      lua_pcall(&luaState, 0, 0, 0)) {
    exit(1); // TODO error handling
  }
  return LuaProgramResult(luaState);
};

bool convert(Selector &selector, bool *) {
  if (!lua_isboolean(&selector.luaState, -1)) {
    return false; // TODO error handling
  }
  bool result = lua_toboolean(&selector.luaState, -1);
  return result;
}

double convert(Selector &selector, double *) {
  int isnum;
  int result = lua_tonumberx(&selector.luaState, -1, &isnum);
  if (!isnum) {
    return 0; // TODO error handling
  }
  return result;
}

int convert(Selector &selector, int *) {
  int isnum;
  int result = lua_tointegerx(&selector.luaState, -1, &isnum);
  if (!isnum) {
    return 0; // TODO error handling
  }
  return result;
}

std::string convert(Selector &selector, std::string *) {
  if (!lua_isstring(&selector.luaState, -1)) {
    return ""; // TODO error handling
  }
  std::string result = lua_tostring(&selector.luaState, -1);
  return result;
}
