require 'pp'

# parameters
minutes = 30 # how long to run the sim
packet_size_bytes = 25
packet_size_bits = packet_size_bytes * 8
channels = [ 12, 11, 10, 9, 8, 7, 6 ]
channel_bits_per_second = [ 0.293 * 1000, 0.537 * 1000, 0.976 * 1000, 1.757 * 1000, 3.125 * 1000, 5.468 * 1000, 9.375 * 1000 ]
channel_time_per_packet = channel_bits_per_second.map{ |bps| packet_size_bits / bps }

# print out header for our TSV output
puts "packets_per_minute\t#{channels.join("\t")}\tall"

# let's gather some data points, based on a few packets/min settins
(100..1000).step(100) do | packets_per_minute |

  # generate random packets over the total time
  packets = []
  ( 1..packets_per_minute * minutes ).each do |i|
    channel = ( rand() * 7 ).to_i # uniformly pick a random channel
    start_time = rand() * 60 * minutes # pick a start time
    end_time = start_time + channel_time_per_packet[ channel ] # compute the end time
    # store this packet as a json object
    packets << { channel: channel, start_time: start_time, end_time: end_time, collision: false }
  end

  # pair all packets to see which collide
  (0..packets.size - 2).each do |i|
    (i+1..packets.size - 1).each do |j|
      packet1 = packets[ i ]
      packet2 = packets[ j ]
      if packet1[ :channel ] == packet2[ :channel ] # we need to be on the same channel
        if( # and we need some overlap in our packet windows
            ( 
              ( packet1[ :end_time ] > packet2[ :start_time ] ) && 
              ( packet1[ :end_time ] < packet2[ :end_time ] ) 
            ) || 
            ( 
              ( packet2[ :end_time ] > packet1[ :start_time ] ) && 
              ( packet2[ :end_time ] < packet1[ :end_time ] ) 
            )
          )
          packets[ i ][ :collision ] = true
          packets[ j ][ :collision ] = true
        end
      end
    end
  end

  # count errors, per channel
  error_counter = [ 0, 0, 0, 0, 0, 0, 0 ]
  all_channels_error_counter = 0
  channel_counter = [ 0, 0, 0, 0, 0, 0, 0 ]
  all_channels_counter = 0
  packets.each do | packet |
    error_counter[ packet[ :channel ] ] += 1 if packet[ :collision ]
    all_channels_error_counter += 1 if packet[ :collision ]
    channel_counter[ packet[ :channel ] ] += 1
    all_channels_counter += 1
  end
  
  # print out these counts as error rates
  print "#{packets_per_minute}"
  (0..6).each do | counter |
    print "\t#{error_counter[ counter ].to_f / channel_counter[ counter ]}"
  end
  print "\t#{all_channels_error_counter.to_f / all_channels_counter}"
  puts

end
