# zeromq-jsonrpc-example
example and expermiment using jsonrpc over zeromq

```
run app-broker
run 3 instances of app -p 5560
run the ruby file to to hit the load balanced rpc servers, client will show which server is responding
it is possible to kill off processes during the client connection
very robust and fast and low memory
```
