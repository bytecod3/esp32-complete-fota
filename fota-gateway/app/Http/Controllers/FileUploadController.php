<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use App\Http\Controllers\MqttController;
use PhpMqtt\Client\Facades\MQTT;

class FileUploadController extends Controller
{
    public function store(Request $request) {

        $mqtt = new MqttController();

        $request->validate([
           'bin_file' => 'required|file|max:10240',
        ]);

        $file = $request->file('bin_file');
        $filename = time().'_'.$file->getClientOriginalName();

        // extract metadata
        $firmware_size = $request->file('bin_file')->getSize();
        $version = $request->input('version');
        $device_id = $request->input('device_id');
        $mode = $request->input('mode');
        $time = 0;

        if($mode == "Scheduled") {
            $time = "2026-06-21T14:30:30Z";
        } 

        // serialize
        $json = [
            'device_id' => $device_id,
            'version'  => $version,
            'size'     => $firmware_size,
            'filename' => $filename,
            'mode'     => $mode,
            'time'     => $time
        ];

        $firmware_metadata = json_encode($json);

        // publish metadata to device
        $mqtt->publish_msg($firmware_metadata);

        // store file in storage/app/bin_files directory
        $file->storeAs('bin_files', $filename, 'public');

        return redirect()->back()->with('success', 'File uploaded successfully');
    }
}
