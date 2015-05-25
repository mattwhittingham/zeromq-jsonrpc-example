require 'ffi-rzmq'

context = ZMQ::Context.new(1)

# Socket to talk to server
puts "Connecting to hello world server..."
requester = context.socket(ZMQ::REQ)
requester.connect("tcp://localhost:5559")

0.upto(9) do |request_nbr|
  puts "Sending request #{request_nbr}..."
  requester.send_string '{"jsonrpc": "2.0", "method": "sayHello", "params":{"name": "Everyone"}, "id":'+request_nbr.to_s+'}'

  reply = ''
  sleep 4
  rc = requester.recv_string(reply)
  
  puts "Received reply #{request_nbr}: [#{reply}]"
end
