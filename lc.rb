#!/usr/bin/ruby

class PRGM
	def self.directory_hash(path, space = '')
		j = 0
		entries = []
		Dir.foreach(path) do |entry|
			next if entry == '..' or entry.start_with?('.')
			full_path = File.join(path, entry)
			if File.directory?(full_path)
				puts "#{space}Entering '#{full_path}':"
				i = directory_hash(full_path, space + '    ')
				puts "#{space}Exiting '#{full_path}': #{i} lines"
				j += i
			elsif entry.end_with?('.c') or 
					entry.end_with?('.cc') or 
					entry.end_with?('.cpp') or 
					entry.end_with?('.h') or
					entry.end_with?('.hpp') or
					entry.end_with?('.s') or
					entry.end_with?('.asm') or
					entry.end_with?('.rb')
				entries << [full_path, entry]
			end
		end

		entries.each do |full_path, entry|
			i = 0
			File.read(full_path).each_line do |line|
				i += 1
			end
			puts "#{space}    #{entry}: #{i} lines"
	
			j += i
		end

		return j
	end
end

c = PRGM.directory_hash(".")
puts "#{c} lines total"

