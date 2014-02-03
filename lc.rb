#!/usr/bin/ruby

c = 0
Dir.foreach('.') do |file|
	next unless file.end_with?('.c') or 
				file.end_with?('.cc') or 
				file.end_with?('.cpp') or 
				file.end_with?('.h') or
				file.end_with?('.hpp')

	i = 0
	File.read(file).each_line do |line|
		i += 1
	end
	puts "#{file}: #{i} lines"

	c += i
end

puts "#{c} lines total"

