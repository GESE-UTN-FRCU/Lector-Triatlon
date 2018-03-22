const express = require('express');
const bodyParser = require('body-parser');
const path = require("path");
const app = express();

app.use(express.static(__dirname + '/public'));
app.use('/jquery', express.static(__dirname + '/node_modules/jquery/dist/'));
app.get('/*', function(req, res){
  res.sendFile(__dirname + '/index.html');
});

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

app.post('/form', function(req, res){
    res.setHeader('Content-Type', 'application/json');

    setTimeout(function(){

        res.send(JSON.stringify({
            id: req.body.id || null,
            tiempo: req.body.tiempo || null
        }));

    }, 1000)

    console.log('Lectura tarjeta: ' + req.body.id + ', lectura tiempo ' + req.body.tiempo);
});


app.listen(3000, () => console.log('Servidor corriendo'))
