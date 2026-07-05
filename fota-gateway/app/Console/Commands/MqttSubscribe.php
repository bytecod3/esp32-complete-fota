<?php

namespace App\Console\Commands;

use Illuminate\Console\Attributes\Description;
use Illuminate\Console\Attributes\Signature;
use Illuminate\Console\Command;
use PhpMqtt\Client\Facades\MQTT;

class MqttSubscribe extends Command
{
    protected $signature = 'mqtt:subscribe {topic}';
    protected $description = 'Subscribe to a given MQTT topic and display messages on terminal';

    public function __construct()
    {
        parent::__construct();
    }

    /**
     * Execute the console command.
     */
    public function handle()
    {
        $topic = $this->argument('topic');
        $mqtt = MQTT::connection();
        $this->info("Subscribing to topic: {$topic}");

        $mqtt->subscribe($topic, function (string $topic, string $message) {
            $this->info(sprintf("Received message on topic [%s]: %s", $topic, $message));
        });

        $mqtt->loop(true);
    }
}
