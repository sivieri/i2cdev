-module(i2cdev).
-export([init/0, setup/1, close/1, read/1, write/2, write_reg8/3, write_reg16/3, read_reg8/2, read_reg16/2]).
-on_load(init/0).

-define(APPNAME, i2cdev).

init() ->
    case code:priv_dir(?APPNAME) of
        {error, _} -> 
            error_logger:format("~w priv dir not found~n", [?APPNAME]),
            exit(error);
        PrivDir ->
            erlang:load_nif(filename:join([PrivDir, "i2cdev_drv"]), 0)
    end.

setup(_Device) ->
    erlang:nif_error(nif_not_loaded).

close(_Device) ->
    erlang:nif_error(nif_not_loaded).

read(_Device) ->
    erlang:nif_error(nif_not_loaded).

write(_Device, _Data) ->
    erlang:nif_error(nif_not_loaded).

write_reg8(_Device, _Register, _Data) ->
    erlang:nif_error(nif_not_loaded).

write_reg16(_Device, _Register, _Data) ->
    erlang:nif_error(nif_not_loaded).

read_reg8(_Device, _Register) ->
    erlang:nif_error(nif_not_loaded).

read_reg16(_Device, _Register) ->
    erlang:nif_error(nif_not_loaded).
