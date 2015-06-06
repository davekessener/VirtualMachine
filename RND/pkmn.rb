#!/usr/bin/ruby

class String
	def is_i?
		!!(self =~ /^[-+]?[0-9]+$/)
	end
end

class Type
	attr_reader :name

	def initialize(name)
		@name = name.upcase
		@weak = []
		@strong = []
		@null = []
	end

	@@normal  = Type.new('normal')
	@@fire    = Type.new('fire')
	@@water   = Type.new('water')
	@@grass   = Type.new('grass')
	@@psychic = Type.new('psychic')
	@@dark    = Type.new('dark')
	@@ghost   = Type.new('ghost')
	@@fight   = Type.new('fight')
	@@ice     = Type.new('ice')
	@@dragon  = Type.new('dragon')
	@@poison  = Type.new('poison')

	def damage_on(target_types)
		return damage_on([target_types]) unless target_types.kind_of?(Array)

		dm = 1; target_types.each do |type|
			dm *= (@weak.include?(type) ? 0.5 : (@strong.include?(type) ? 2 : (@null.include?(type) ? 0 : 1)))
		end; return dm
	end

	def set_effect!(weak, strong, null)
		@weak, @strong, @null = weak, strong, null
	end

	def self.init
		@@normal.set_effect!([@@fight, @@dragon], [], [@@ghost])
		@@fire.set_effect!([@@water, @@dragon], [@@grass, @@ice], [])
		@@water.set_effect!([@@grass, @@dragon], [@@fire], [])
		@@grass.set_effect!([@@fire, @@ice, @@dragon], [@@water], [])
		@@psychic.set_effect!([], [@@ghost, @@fight, @@poison], [@@dark])
		@@dark.set_effect!([@@fight], [@@psychic], [])
		@@ghost.set_effect!([@@dark, @@fight], [@@psychic, @@ghost], [@@normal])
		@@fight.set_effect!([@@psychic, @@dragon], [@@normal, @@dark], [@@ghost])
		@@ice.set_effect!([@@fire, @@water], [@@grass, @@dragon], [])
		@@dragon.set_effect!([@@dark], [@@dragon, @@ice], [])
		@@poison.set_effect!([@@ghost, @@poison], [@@grass], [])
	end

	def self.normal;  return @@normal;  end
	def self.fire;    return @@fire;    end
	def self.water;   return @@water;   end
	def self.grass;   return @@grass;   end
	def self.psychic; return @@psychic; end
	def self.dark;    return @@dark;    end
	def self.ghost;   return @@ghost;   end
	def self.fight;   return @@fight;   end
	def self.ice;     return @@ice;     end
	def self.dragon;  return @@dragon;  end
	def self.poison;  return @@poison;  end
end

class Species
	attr_reader :name, :id, :b_atk, :b_def, :b_init, :b_hp
	attr_reader :types, :attacks
	@@IDs = 0
	@@all = [] 

	def initialize(name, b_atk, b_def, b_init, b_hp, atks, type1, type2 = nil)
		@name = name.upcase
		@id = @@IDs

		@@IDs += 1
		@@all[@id] = self

		@b_atk  = b_atk
		@b_def  = b_def
		@b_init = b_init
		@b_hp   = b_hp

		@attacks = atks
		@types = [type1, type2].reject do |e| e.nil? end
	end

	def self.init
		Species.new('bulbasaur', 49, 49, 45, 45, [Attacks.tackle, Attacks.sludge_bomb, Attacks.razor_leaf], Type.grass, Type.poison)
		Species.new('charmander', 52, 43, 65, 39, [Attacks.quick_attack, Attacks.flamethrower], Type.fire)
		Species.new('squirtle', 48, 65, 43, 44, [Attacks.tackle, Attacks.surf, Attacks.ice_beam], Type.water)
		Species.new('rattata', 56, 35, 72, 30, [Attacks.quick_attack], Type.normal)
		Species.new('abra', 20, 15, 90, 25, [Attacks.psychic], Type.psychic)
		Species.new('houndour', 60, 30, 65, 45, [Attacks.flamethrower, Attacks.bite], Type.dark, Type.fire)
		Species.new('gastly', 35, 30, 80, 30, [Attacks.shadow_ball, Attacks.psychic], Type.ghost, Type.poison)
		Species.new('hitmonchan', 105, 79, 76, 50, [Attacks.low_kick], Type.fight)
		Species.new('lapras', 85, 80, 60, 130, [Attacks.surf, Attacks.ice_beam], Type.water, Type.ice)
		Species.new('bagon', 75, 60, 50, 45, [Attacks.draco_meteor, Attacks.flamethrower], Type.dragon)
	end

	def self.random; return @@all.sample; end
end

class Attack
	attr_reader :id, :name, :type, :strength, :accuracy
	@@IDs = 0
	@@all = []

	def initialize(name, type, str, acc)
		@id = @@IDs
		@name = name.upcase
		@type = type
		@strength = str
		@accuracy = acc

		@@IDs += 1
		@@all[@id] = self
	end

	def attack(user, target)
		stab = user.types.include?(@type) ? 1.5 : 1
		pow = user.attack / target.defense
		type = @type.damage_on(target.types)
		luck = (rand*15 + 85) / 100
		attack = @strength

		return (((2 * user.level + 10) / 250 * pow * attack + 2) * stab * type * luck + 0.5).round
	end

	def hit?(user, target)
		return (@accuracy <= 0 or rand < hit_chance(user, target))
	end

	def hit_chance(user, target)
		return @accuracy / 100 * 1 / 1
	end
end

class Attacks
	class << self
		attr_reader :tackle, :quick_attack, :flamethrower, :surf, :razor_leaf, :psychic, :bite, :shadow_ball
		attr_reader :low_kick, :ice_beam, :draco_meteor, :sludge_bomb
	end

	@tackle = Attack.new('tackle', Type.normal, 35, 95)
	@quick_attack =	Attack.new('quick attack', Type.normal, 40, 100)
	@flamethrower =	Attack.new('flamethrower', Type.fire, 95, 100)
	@surf = Attack.new('surf', Type.water, 90, 100)
	@razor_leaf = Attack.new('razor leaf', Type.grass, 55, 85)
	@psychic = Attack.new('psychic', Type.psychic, 90, 100)
	@bite = Attack.new('bite', Type.dark, 40, 100)
	@shadow_ball = Attack.new('shadow ball', Type.ghost, 85, 100)
	@low_kick = Attack.new('low kick', Type.fight, 35, 100)
	@ice_beam = Attack.new('ice beam', Type.ice, 90, 100)
	@draco_meteor = Attack.new('draco meteor', Type.dragon, 140, 90)
	@sludge_bomb = Attack.new('sludge bomb', Type.poison, 90, 100)
end

class Pkmn
	attr_reader :species, :level, :attack, :defense, :speed, :cur_hp, :max_hp

	def initialize(species, level)
		@species = species
		@level = level

		@attack = c_stat(@species.b_atk)
		@defense = c_stat(@species.b_def)
		@speed = c_stat(@species.b_init)
		@max_hp = (((rand * 32).round + 2 * @species.b_hp + 100) * @level / 100) + 10
		@cur_hp = @max_hp
	end

	def cur_hp=(chp)
		@cur_hp = chp < 0 ? 0 : chp
	end

	def do_attack(atk, target)
		unless atk.hit?(self, target)
			puts "But it missed ..."
		else
			dmg = atk.attack(self, target)

			if(dmg <= 0)
				puts "But it failed!"
			else
				target.cur_hp -= dmg
			end
		end
	end

	def types; return @species.types; end
	def attacks; return @species.attacks; end
	def c_stat(b); return (((rand * 32).round + 2 * b) * @level / 100) + 5; end
	def fainted?; return @cur_hp == 0; end
end

class Main
	def self.execute
		Type.init
		Species.init

		mine = Pkmn.new(Species.random, (rand * 8 + 61).round)
		oppo = Pkmn.new(Species.random, (rand * 8 + 61).round)

		until mine.fainted? or oppo.fainted?
			puts "PLAYER: #{mine.species.name} #{mine.cur_hp}/#{mine.max_hp}"
			puts "OPPONE: #{oppo.species.name} #{oppo.cur_hp}/#{oppo.max_hp}"

			puts "Choose attack:"
			mine.attacks.each_index do |i|
				puts "  #{i}: #{mine.attacks[i].name}"
			end

			print "\n> "
			i = gets
			break unless i.is_i?

			j = (rand * (oppo.attacks.length - 1)).round

			if mine.speed > oppo.speed or (mine.speed == oppo.speed and rand <= 0.5)
				puts "#{mine.species.name} uses #{mine.attacks[i.to_i].name}!"
				mine.do_attack(mine.attacks[i.to_i], oppo)
				break if oppo.fainted? or mine.fainted?
				puts "#{oppo.species.name} (foe) uses #{oppo.attacks[j.to_i].name}!"
				oppo.do_attack(oppo.attacks[j.to_i], mine)
			else
				puts "#{oppo.species.name} (foe) uses #{oppo.attacks[j.to_i].name}!"
				oppo.do_attack(oppo.attacks[j.to_i], mine)
				break if oppo.fainted? or mine.fainted?
				puts "#{mine.species.name} uses #{mine.attacks[i.to_i].name}!"
				mine.do_attack(mine.attacks[i.to_i], oppo)
			end
		end

		if mine.fainted?
			puts "YOU LOOSE!"
		elsif oppo.fainted?
			puts "YOU WIN!"
		else
			puts "GOODBYE"
		end
	end
end

Main.execute

