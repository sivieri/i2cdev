#include <wiringPiI2C.h>
#include <erl_nif.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>

typedef struct {
  ErlNifEnv *env;
  int fd;
  int device;
} device_t;

extern int errno;

static ErlNifResourceType *resource;

static void pseudo_destructor(ErlNifEnv* env, void* obj) {
	// not used
}

static int load(ErlNifEnv* env, void** priv, ERL_NIF_TERM load_info) {
	resource = enif_open_resource_type(env, NULL, "i2cdev", &pseudo_destructor, ERL_NIF_RT_CREATE, NULL);

	return 0;
}

static ERL_NIF_TERM setup_1(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
	unsigned int d;
	device_t *device;
	int error;
	ERL_NIF_TERM res;

	if (argc != 1 || !enif_is_number(env, argv[0])) {
		return enif_make_badarg(env);
	}
	if (!enif_get_uint(env, argv[0], &d)) {
		return enif_make_badarg(env);
	}
	device = (device_t *) enif_alloc_resource(resource, sizeof(device_t));
	device->env = env;
	device->device = d;
	device->fd = wiringPiI2CSetup(d);
	if (device->fd == -1) {
		error = errno;
		enif_release_resource(device);
		return enif_make_tuple2(env, enif_make_atom(env, "error"), enif_make_int(env, error));
	}
	res = enif_make_tuple2(env, enif_make_atom(env, "ok"), enif_make_resource(env, device));
	enif_release_resource(device);

	return res;
}

static ERL_NIF_TERM close_1(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    device_t *device;

	if (argc != 1) {
    	return enif_make_badarg(env);
    }
	if (!enif_get_resource(env, argv[0], resource, (void **) &device)) {
		return enif_make_badarg(env);
	}
	close(device->fd);
	enif_release_resource(device);

	return enif_make_atom(env, "ok");
}

static ERL_NIF_TERM read_1(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
	device_t *device;
	int result;

	if (argc != 1) {
		return enif_make_badarg(env);
	}
	if (!enif_get_resource(env, argv[0], resource, (void **) &device)) {
		return enif_make_badarg(env);
	}
	result = wiringPiI2CRead(device->fd);
	if (result == -1) {
		result = errno;
		enif_release_resource(device);
		return enif_make_tuple2(env, enif_make_atom(env, "error"), enif_make_int(env, result));
	}
	enif_release_resource(device);

	return enif_make_tuple2(env, enif_make_atom(env, "ok"), enif_make_int(env, result));
}

static ERL_NIF_TERM read_reg8_2(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
	device_t *device;
	int result, reg;

	if (argc != 2 || !enif_is_number(env, argv[1])) {
		return enif_make_badarg(env);
	}
	if (!enif_get_resource(env, argv[0], resource, (void **) &device)) {
		return enif_make_badarg(env);
	}
	if (!enif_get_int(env, argv[1], &reg)) {
		return enif_make_badarg(env);
	}
	result = wiringPiI2CReadReg8(device->fd, reg);
	if (result == -1) {
		result = errno;
		enif_release_resource(device);
		return enif_make_tuple2(env, enif_make_atom(env, "error"), enif_make_int(env, result));
	}
	enif_release_resource(device);

	return enif_make_tuple2(env, enif_make_atom(env, "ok"), enif_make_int(env, result));
}

static ERL_NIF_TERM read_reg16_2(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
	device_t *device;
	int result, reg;

	if (argc != 2 || !enif_is_number(env, argv[1])) {
		return enif_make_badarg(env);
	}
	if (!enif_get_resource(env, argv[0], resource, (void **) &device)) {
		return enif_make_badarg(env);
	}
	if (!enif_get_int(env, argv[1], &reg)) {
		return enif_make_badarg(env);
	}
	result = wiringPiI2CReadReg16(device->fd, reg);
	if (result == -1) {
		result = errno;
		enif_release_resource(device);
		return enif_make_tuple2(env, enif_make_atom(env, "error"), enif_make_int(env, result));
	}
	enif_release_resource(device);

	return enif_make_tuple2(env, enif_make_atom(env, "ok"), enif_make_int(env, result));
}

static ERL_NIF_TERM write_2(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
	device_t *device;
	int data, result;

	if (argc != 2 || !enif_is_number(env, argv[1])) {
		return enif_make_badarg(env);
	}
	if (!enif_get_resource(env, argv[0], resource, (void **) &device)) {
		return enif_make_badarg(env);
	}
	if (!enif_get_int(env, argv[1], &data)) {
		return enif_make_badarg(env);
	}
	result = wiringPiI2CWrite(device->fd, data);
	if (result == -1) {
		result = errno;
		enif_release_resource(device);
		return enif_make_tuple2(env, enif_make_atom(env, "error"), enif_make_int(env, result));
	}
	enif_release_resource(device);

	return enif_make_atom(env, "ok");
}

static ERL_NIF_TERM write_reg8_3(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
	device_t *device;
	int data, reg, result;

	if (argc != 3 || !enif_is_number(env, argv[1]) || !enif_is_number(env, argv[2])) {
		return enif_make_badarg(env);
	}
	if (!enif_get_resource(env, argv[0], resource, (void **) &device)) {
		return enif_make_badarg(env);
	}
	if (!enif_get_int(env, argv[1], &reg)) {
		return enif_make_badarg(env);
	}
	if (!enif_get_int(env, argv[2], &data)) {
		return enif_make_badarg(env);
	}
	result = wiringPiI2CWriteReg8(device->fd, reg, data);
	if (result == -1) {
		result = errno;
		enif_release_resource(device);
		return enif_make_tuple2(env, enif_make_atom(env, "error"), enif_make_int(env, result));
	}
	enif_release_resource(device);

	return enif_make_atom(env, "ok");
}

static ERL_NIF_TERM write_reg16_3(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
	device_t *device;
	int data, reg, result;

	if (argc != 3 || !enif_is_number(env, argv[1]) || !enif_is_number(env, argv[2])) {
		return enif_make_badarg(env);
	}
	if (!enif_get_resource(env, argv[0], resource, (void **) &device)) {
		return enif_make_badarg(env);
	}
	if (!enif_get_int(env, argv[1], &reg)) {
		return enif_make_badarg(env);
	}
	if (!enif_get_int(env, argv[2], &data)) {
		return enif_make_badarg(env);
	}
	result = wiringPiI2CWriteReg16(device->fd, reg, data);
	if (result == -1) {
		result = errno;
		enif_release_resource(device);
		return enif_make_tuple2(env, enif_make_atom(env, "error"), enif_make_int(env, result));
	}
	enif_release_resource(device);

	return enif_make_atom(env, "ok");
}

static ErlNifFunc i2cdev_NIFs[] = {
    {"setup", 1, &setup_1},
    {"close", 1, &close_1},
    {"read", 1, &read_1},
    {"read_reg8", 2, &read_reg8_2},
    {"read_reg16", 2, &read_reg16_2},
    {"write", 2, &write_2},
	{"write_reg8", 3, &write_reg8_3},
	{"write_reg16", 3, &write_reg16_3},
};

ERL_NIF_INIT(i2cdev, i2cdev_NIFs, load, NULL, NULL, NULL);
