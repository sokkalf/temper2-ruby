Gem::Specification.new do |s|
  s.name = 'temper2-ruby'
  s.version = '0.0.1'
  s.date = '2013-04-17'
  s.summary = 'Read temperatures from TEMPer2 sensor'
  s.authors = ['Christian Lønaas']
  s.email = 'christian.lonaas@discombobulator.org'
  s.files = Dir.glob('lib/**/*.rb') +
            Dir.glob('ext/**/*.{c,h,rb}')
  s.extensions = ['ext/temper2/extconf.rb']
 end

