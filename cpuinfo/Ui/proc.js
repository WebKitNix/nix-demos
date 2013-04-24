var gauge;
var gaugeData;
var gaugeOptions;
function drawGauge() {
  dataTable = [['Label', 'Value']];

  for (var i = 0; i < window._getCpus(); i++) {
    dataTable.push(['CPU '+ (i + 1), 0]);
  };

  gaugeData = google.visualization.arrayToDataTable(dataTable);

  gauge = new google.visualization.Gauge(document.getElementById('gauge'));
  gaugeOptions = {
      min: 0,
      max: 100,
      yellowFrom: 60,
      yellowTo: 85,
      redFrom: 85,
      redTo: 100,
      minorTicks: 10
  };
  gauge.draw(gaugeData, gaugeOptions);
}

function changeLoad(cpu, load) {
  gaugeData.setValue(cpu, 1, load);
  gauge.draw(gaugeData, gaugeOptions);
}

google.setOnLoadCallback(drawGauge);

setInterval(function() {
  for (var i = 0; i < window._getCpus(); i++) {
    changeLoad(i, window._getLoad(i));
  };
}, 100);
