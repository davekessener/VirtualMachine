#!/usr/bin/ruby

class PRGM
	@@longest = ['', -1]

	def self.directory_hash(path, space = '')
		j = 0
		entries = []
		Dir.foreach(path) do |entry|
			next if entry == '..' or entry.start_with?('.')
			next if entry == 'boost'
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
					entry.end_with?('.sh') or
					entry.end_with?('.py') or
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

			if @@longest[1] < i
				@@longest = [full_path, i]
			end
	
			j += i
		end

		return j
	end

	def self.longest
		return @@longest
	end
end

c = PRGM.directory_hash(".")
puts "#{c} lines total"
puts "'#{PRGM.longest[0]}' is the longest fil with #{PRGM.longest[1]} lines of code."

