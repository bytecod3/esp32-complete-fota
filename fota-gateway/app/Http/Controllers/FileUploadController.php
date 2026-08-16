<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use App\Http\Controllers\MqttController;
use PhpMqtt\Client\Facades\MQTT;
use Illuminate\Support\Facades\Storage;



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
        $size = $request->file('bin_file')->getSize();
        $version = $request->input('version');
        $device_id = $request->input('device_id');
        $mode = $request->input('mode');
        $time = 0;
        $bin_folder = "/bin_files/";
        $url = Storage::disk('public')->url($bin_folder.$filename);

        if($mode == "Scheduled") {
            $time = "2026-06-21T14:30:30Z";
        } 

        // cache the firmware metadata
        cache()->put('latest_firmware', [
            'version' => $version,
            'size' => $size,
            'url' => $url
        ], now()->addDays(30));

        // serialize
        $json = [
            'device_id' => $device_id,
            'version'  => $version,
            'size'     => $size,
            'filename' => $filename,
            'mode'     => $mode,
            'time'     => $time,
            'url'      => $url
        ];

        $firmware_metadata = json_encode($json);

        // publish metadata to device
        $mqtt->publish_msg($firmware_metadata);

        // store file in storage/app/bin_files directory
        $file->storeAs('bin_files', $filename, 'public');

        return redirect()->back()->with('success', 'File uploaded successfully');
    }

    // This will send update metadata to the device
    public function expose_metadata() {
        $metadata = cache()->get('latest_firmware');
        $metadata_json = json_encode($metadata);

        if($metadata === null) {
            return response()->json([
                'error' => 'No firmware available'
            ], 404);
        }

        return response()->json([
            'version' => $metadata['version'],
            'size' => $metadata['size'],
            'url' => $metadata['url']            
            ])
        ->header('Content-Length', strlen($metadata_json));

    }
}
