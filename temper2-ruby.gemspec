Gem::Specification.new do |s|
  s.files = Dir.glob('lib/**/*.rb') +
            Dir.glob('ext/**/*.{c,h,rb}')
  s.extensions = ['ext/temper2/extconf.rb']
 end

