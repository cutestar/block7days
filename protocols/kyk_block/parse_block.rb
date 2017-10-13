#!/usr/bin/env ruby

require 'bitcoin'

raw_blk = File.open(ARGV[0], 'rb') {|f| f.read }

blk = Bitcoin::Protocol::Block.new(raw_blk)
puts blk.hash
puts blk.tx.count
 puts blk.to_hash
# puts Bitcoin::Protocol::Block.from_json( blk.to_json )
puts blk.to_json
