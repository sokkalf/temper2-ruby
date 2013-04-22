#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <linux/limits.h>
#include <sys/types.h>
#include <inttypes.h>
#include <usb.h>
#include <errno.h>
#include <ruby.h>

/*
 * Temper.c by Robert Kavaler (c) 2009 (relavak.com)
 * All rights reserved.
 *
 * Modified by Sylvain Leroux (c) 2012 (sylvain@chicoree.fr)
 * 
 * Ruby gem by Christian Lønaas (c) 2012 (christian.lonaas@discombobulator.org)
 *
 * Temper driver for linux. This program can be compiled either as a library
 * or as a standalone program (-DUNIT_TEST). The driver will work with some
 * TEMPer usb devices from RDing (www.PCsensor.com).
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Robert kavaler BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#include "comm.h"

#if !defined TEMPER_TIMEOUT
#define TEMPER_TIMEOUT 1000	/* milliseconds */
#endif

#if !defined TEMPER_DEBUG
#define TEMPER_DEBUG 0
#endif

#define INNER_SENSOR 0
#define OUTER_SENSOR 1
#define SENSOR_ERROR 255

float get_inner_temp();
float get_outer_temp();

static VALUE read_outer_sensor(VALUE self)
{
    float outer = get_outer_temp();
    if (outer == SENSOR_ERROR)
	rb_raise(rb_eRuntimeError, "can't read from sensor");
    else
	return rb_float_new(outer);

    return Qnil;
}

static VALUE read_inner_sensor(VALUE self)
{
    float inner = get_inner_temp();
    if (inner == SENSOR_ERROR)
	rb_raise(rb_eRuntimeError, "can't read from sensor");
    else
	return rb_float_new(inner);

    return Qnil;
}

void Init_temper2(void)
{
    VALUE c = rb_define_class("Temper2", rb_cObject);

    rb_define_singleton_method(c, "read_outer_sensor", read_outer_sensor,
			       0);
    rb_define_singleton_method(c, "read_inner_sensor", read_inner_sensor,
			       0);
}

TemperData *get_temper_data()
{
    Temper *t;
    int ret;

    usb_set_debug(0);
    usb_init();
    usb_find_busses();
    usb_find_devices();

    t = TemperCreateFromDeviceNumber(0, TEMPER_TIMEOUT, TEMPER_DEBUG);
    if (!t) {
	return NULL;
    }

    TemperSendCommand8(t, 0x01, 0x80, 0x33, 0x01, 0x00, 0x00, 0x00, 0x00);
    if (0) {
	unsigned char buf[8];
	TemperInterruptRead(t, buf, sizeof(buf));
    } else {
	TemperData temp[2];
	TemperData *data = malloc(sizeof(TemperData) * 2);
	const unsigned int count = sizeof(temp) / sizeof(TemperData);
	ret = TemperGetData(t, data, count);
	TemperFree(t);
	return data;
    }

    TemperFree(t);

    return NULL;
}


float get_inner_temp()
{
    TemperData *t = get_temper_data();;

    if (t == NULL)
	return SENSOR_ERROR;

    float temperature = t[INNER_SENSOR].value;
    free(t);

    return temperature;
}

float get_outer_temp()
{
    TemperData *t = get_temper_data();

    if (t == NULL)
	return SENSOR_ERROR;

    float temperature = t[OUTER_SENSOR].value;
    free(t);
    return temperature;
}
