#ifndef SCRIPTED_INSPECTABLE_HPP
#define SCRIPTED_INSPECTABLE_HPP

#include "../Thing.hpp"
#include "ScriptedThing.hpp"
#include "LuaHelpers.hpp"

class ScriptedInspectable : public Inspectable
{
 
    const std::string onInspect(const std::shared_ptr<Thing> &owner, const std::shared_ptr<Thing> &inspector) override 
    {
        const auto& L = std::static_pointer_cast<ScriptedThing>(owner) -> L;

        lua_getglobal(  L, owner -> name.c_str() );

        lua_getfield( L, -1, "onInspect" );

        if (!lua_isfunction(L, -1)) return Inspectable::onInspect(owner, inspector);

        lua_pushlightuserdata(L, owner.get());
        lua_pushlightuserdata(L, inspector.get());
        
        CheckLua(L, lua_pcall(L, 2, 1, 0));

        assert(lua_isstring(L, -1));

        return  Inspectable::onInspect(owner, inspector) + std::string(lua_tostring(L, -1));
    }

};


#endif