#!/usr/bin/ruby

class Node
	attr_reader :ID, :incs, :libs, :deps, :objs, :exec, :cmds
	@@libraries = {:math => '-lm'}
	@@cflags = '-Wall -ggdb -O0 -Wno-psabi'
	@@macros = '-DUNIX'

	def initialize(fn)
		@ID = fn
		@cmds = []
		@mins = []
		@incs, @libs = get_incs(fn)
		@incs, @deps = make_dependencies(@incs)
		@objs = @deps.dup
		@exec = is_executable?(fn)

		resolve_instructions()

		@cmds.reject!(&:nil?)
		@incs.reject!(&:nil?) 
		@deps.reject!(&:nil?)
		@objs.reject!(&:nil?)

		@incs.uniq! 
		@deps.uniq!
		@objs.uniq!
	end

	def resolve_instructions
		@mins.each do |ins|
			i = ins[0]
			ins = ins[1..-1]

			case i
				when 'i'
					@cmds << ins
				when 'e'
					eval(ins)
				when 'd'
					i = ins[0]
					ins = ins[1..-1].gsub(/[\t\n]+/, '')
					@deps.reject! { |e| e === ins } if i === '-'
					@deps << ins if i === '+'
				when 'D'
					i = ins[0]
					ins = ins[1..-1].gsub(/[\t\n]+/, '')
					@objs.reject! { |e| e === ins } if i === '-'
					@objs << ins if i === '+'
			end
		end
	end

	def make_dependencies(incs)
		cf = []
		hf = []

		incs.each do |file|
			if file.end_with?('.c')
				cf << file
			elsif file.end_with?('.h')
				file.gsub!(/\.h$/, '')

				hf << "#{file}.h"
				cf << "#{file}.c" if File.exist?("#{file}.c")
			end
		end

		return [hf, cf]
	end

	def get_incs(fn, fs = [], ls = [])
		if not fs.include?(fn)
			fs << fn
	
			File.read(fn).each_line do |line|
				@mins << line[3..-1] if line.start_with?('//#')
				next if not line.match(/^[ \t]*#include/)
				
				line.gsub!(/^[ \t]*#include[ \t]+([\"<].+\.h[\">])[.\n]*$/, '\1')
	
				if line.match(/^<.*>$/)
					line.gsub!(/^<(.*)\.h>$/, '\1')
					ls << @@libraries[line.intern] if @@libraries.key?(line.intern)
				elsif line.match(/^\".*\"$/)
					line.gsub!(/^\"(.*)\"$/, '\1')
					fs, ls = get_incs(line, fs, ls)
				end
			end
		end

		return [fs.uniq, ls.uniq] 
	end

	def is_executable?(fn)
		line1 = File.read(fn)
		line2 = line1.gsub(/\/\*[.\n]*?\*\//, '')
		lines = line2.split(/\n/)

		lines.each_index do |i|
			lines[i].gsub!(/([^\\])\"[.^\"]*?[^\\]\"/, '\1')
			lines[i].gsub!(/\/\/.*$/, '')

			return true if lines[i].match(/(int|void)[ \t]+main\(.*\)/)
		end

		return false
	end

	def self.build(flags = @@cflags, macros = @@macros)
		head = "CC=cc\nCFLAGS=#{flags}\nMACROS=#{macros}\n\n"
		obj = ""
		exec = ""
		all = []
		objs = []

		Dir.foreach('.') do |file|
			next if not file.end_with?('.c')

			node = Node.new(file)

			obj += "#{node.ID}.o: #{node.ID} #{node.incs.join(' ')}\n\t$(CC) $(CFLAGS) $(MACROS) -c #{node.ID} -o $@\n\n"
			objs << "#{node.ID}.o"

			if(node.exec)
				objs += node.objs.map { |e| e + '.o' } unless node.objs.nil?
				ds = node.deps.join('.o ') + '.o' 
				os = node.objs.join('.o ') + '.o'
				exec += "#{node.ID[0..-3]}: #{ds}\n"
				
				node.cmds.each do |cmd|
					exec += eval(cmd)
				end

				exec +="\t$(CC) $(CFLAGS) #{os} -o $@ #{node.libs.join(' ')}\n\n"

				all << node.ID[0..-3]
			end
		end

		objs.uniq!

		head += "all: #{all.join(' ')}\n\n#{exec}\n#{obj}\n"
		head += "clean:\n\trm -f #{objs.join(' ')}\n\n"
		head += "remove: clean\n\trm -f #{all.join(' ')}\n\n"

		puts head.gsub(/ +/, ' ')
	end
end

Node.build

