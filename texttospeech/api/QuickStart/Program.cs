/*
 * Copyright (c) 2018 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */

// [START tts_quickstart]

using System;
using System.IO;
using Google.Cloud.TextToSpeech.V1;

public class QuickStart
{
    public static void Main(string[] args)
    {
        // Instantiate a client
        TextToSpeechClient client = TextToSpeechClient.Create();

        // Set the text input to be synthesized.
        SynthesisInput input = new SynthesisInput
        {
            Text = "Bước 3: Với thời gian tập trung bình 20 đến 30 phút mỗi lần, 5 phút đầu bạn đi bộ và ấn nút Speed cộng để tăng tốc độ với vận tốc từ 1 đến 5 kilomet trên giờ để làm nóng cơ thể.Bạn có thể theo dõi các thông số như vận tốc, quãng đường, nhịp tim, lượng calo độ dốc trên màn hình"
        };

        // Build the voice request, select the language code ("en-US"),
        // and the SSML voice gender ("neutral").
        VoiceSelectionParams voice = new VoiceSelectionParams
        {
            LanguageCode = "vi-VN",
            SsmlGender = SsmlVoiceGender.Female,
            Name = "vi-VN-Wavenet-A"
        };

        // Select the type of audio file you want returned.
        AudioConfig config = new AudioConfig
        {
            SpeakingRate = 0.8,
            AudioEncoding = AudioEncoding.Mp3
        };

        // Perform the Text-to-Speech request, passing the text input
        // with the selected voice parameters and audio file type
        var response = client.SynthesizeSpeech(new SynthesizeSpeechRequest
        {
            Input = input,
            Voice = voice,
            AudioConfig = config
        });

        // Write the binary AudioContent of the response to an MP3 file.
        using (Stream output = File.Create("sample.mp3"))
        {
            response.AudioContent.WriteTo(output);
            Console.WriteLine($"Audio content written to file 'sample.mp3'");
        }
    }
}
// [END tts_quickstart]