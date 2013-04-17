require 'temper2-ruby/temper2'

out_temp = Temper2::read_outer_sensor
in_temp = Temper2::read_inner_sensor

puts 'Outside : ' + out_temp.to_s
puts 'Inside : ' + in_temp.to_s

