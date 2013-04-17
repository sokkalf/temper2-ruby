require 'mkmf'

have_library('usb')
have_library('temper2')

create_makefile('temper2-ruby/temper2')

