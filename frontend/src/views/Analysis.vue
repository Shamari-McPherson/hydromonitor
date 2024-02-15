<template>
    <v-container class="vg-surface" fluid align=" center" color=" surface">
      <v-row class="">
        <v-col>
          <v-sheet class="pa-2">
            <p>Enter date range for Analysis</p>
          </v-sheet>
          <v-divider></v-divider>
          <br />
          <v-text-field
          label="Start date" type="Date"  solo-inverted class="mr-5" max-width="300px" flat v-model="start"
          ></v-text-field>
          <v-text-field
            v-model="end"
            label="End date"
            type="Date"
            dense
            solo-inverted
            class="mr-5"
            :style="{ maxWidth: '300px' }"
            flat
          ></v-text-field>
          <br />
          <v-btn
            text="Analyze"
            @click="
              updateLineCharts();
              updateCards();
              updateHistogramCharts();
              updateScatter();
            "
            color="primary"
            variant="tonal"
          ></v-btn>
        </v-col>
        <v-col cols="3" align="center">
          <v-card
            title="Temperature"
            width="250"
            variant="outlined"
            color="primary"
            density="compact"
            rounded="lg"
          >
            <v-card-item class="mb-n5">
              <v-chip-group
                class="d-flex flex-row justify-center"
                color="primaryContainer"
                variant="flat"
              >
                <v-tooltip text="Min" location="start">
                  <template v-slot:activator="{ props }">
                    <v-chip v-bind="props">{{ temperature.min }}</v-chip>
                  </template>
                </v-tooltip>
  
                <v-tooltip text="Range" location="top">
                  <template v-slot:activator="{ props }">
                    <v-chip v-bind="props">{{ temperature.range }}</v-chip>
                  </template>
                </v-tooltip>
                <v-tooltip text="Max" location="end">
                  <template v-slot:activator="{ props }">
                    <v-chip v-bind="props">{{ temperature.max }}</v-chip>
                  </template>
                </v-tooltip>
              </v-chip-group>
            </v-card-item>
            <v-card-item align="center">
              <span class="text-h1 text-primary font-weight-bold">
                {{ temperature.avg }}
              </span>
            </v-card-item>
          </v-card>
        </v-col>
        <v-col cols="3" align="center">
          <v-card title="Humidity" width="250" variant="outlined" color="primary" density="compact" rounded="lg"
          >
            <v-card-item class="mb-n5">
              <v-chip-group class="d-flex flex-row justify-center" color="primaryContainer" variant="flat"
              >
                <v-tooltip text="Min" location="start">
                  <template v-slot:activator="{ props }">
                    <v-chip v-bind="props">{{ humidity.min }}</v-chip>
                  </template>
                </v-tooltip>
  
                <v-tooltip text="Range" location="top">
                  <template v-slot:activator="{ props }">
                    <v-chip v-bind="props">{{ humidity.range }}</v-chip>
                  </template>
                </v-tooltip>
                <v-tooltip text="Max" location="end">
                  <template v-slot:activator="{ props }">
                    <v-chip v-bind="props">{{ humidity.max }}</v-chip>
                  </template>
                </v-tooltip>
              </v-chip-group>
            </v-card-item>
            <v-card-item align="center">
              <span class="text-h1 text-primary font-weight-bold">
                {{ humidity.avg }}
              </span>
            </v-card-item>
          </v-card>
        </v-col>
      </v-row>
      <v-row class="row">
        <v-col cols="12">
          <figure class="highcharts-figure">
            <div id="container"></div>
          </figure>
        </v-col>
        <v-col cols="12">
          <figure class="highcharts-figure">
            <div id="container0"></div>
          </figure>
        </v-col>
      </v-row>
      <v-row class="row">
        <v-col class="col1" cols="12">
          <figure class="highcharts-figure">
            <div id="container1"></div>
          </figure>
        </v-col>
        <v-col cols="12">
          <figure class="highcharts-figure">
            <div id="container2"></div>
          </figure>
        </v-col>
        <v-col cols="12">
          <figure class="highcharts-figure">
            <div id="container3"></div>
          </figure>
        </v-col>
      </v-row>
    </v-container>
  </template>
  
  <script setup>
  /** JAVASCRIPT HERE */
  
  import Highcharts from "highcharts";
  import more from "highcharts/highcharts-more";
  import Exporting from "highcharts/modules/exporting";
  import { withDirectives } from "vue";
  Exporting(Highcharts);
  more(Highcharts);
  
  // IMPORTS
  import { useMqttStore } from "@/store/mqttStore"; // Import Mqtt Store
  import { storeToRefs } from "pinia";
  
  import { useAppStore } from "@/store/appStore";
  import {
    ref,
    reactive,
    watch,
    onMounted,
    onBeforeUnmount,
    computed,
  } from "vue";
  import { useRoute, useRouter } from "vue-router";
  
  // VARIABLES
  const Mqtt = useMqttStore();
  const AppStore = useAppStore();
  const router = useRouter();
  const route = useRoute();
  var start = ref(null);
  var end = ref(null);
  var temperature = reactive({ min: 0, max: 0, avg: 0, range: 0 });
  var humidity = reactive({ min: 0, max: 0, avg: 0, range: 0 });
  const tempHiLine = ref(null); // Chart object
  const humLine = ref(null); // Chart object
  const histo = ref(null); // Chart object
  const tempHiScat = ref(null); // Chart object
  const humScat = ref(null); // Chart object
  // FUNCTIONS
  
  const CreateCharts = async () => {
    // TEMPERATURE CHART
    tempHiLine.value = Highcharts.chart("container", {
      chart: { zoomType: "x" },
      title: { text: "Air Temperature and Heat Index Analysis", align: "left" },
      subtitle: {
        text:
          " The heat index, also known as the apparent temperature, is a measure that combines air temperature and relative humidity to assess how hot it feels to the human body. " +
          "The relationship between heat index and air temperature is influenced by humidity levels. As humidity increases, the heat" +
          "index also rises, making the perceived temperature higher than the actual air temperature.",
      },
      yAxis: {
        title: {
          text: "Air Temperature & Heat Index",
          style: { color: "#000000" },
        },
        labels: { format: "{value} °C" },
      },
  
      tooltip: {
        pointFormat: "Heatindex: {point.x} °C <br/> Temperature: {point.y} °C",
      },
      xAxis: {
        type: "datetime",
        title: { text: "Time", style: { color: "#000000" } },
      },
      tooltip: { shared: true },
      series: [
        {
          name: "Temperature",
          type: "line",
          data: [],
          turboThreshold: 0,
          color: Highcharts.getOptions().colors[0],
        },
        {
          name: "Heat Index",
          type: "line",
          data: [],
          turboThreshold: 0,
          color: Highcharts.getOptions().colors[1],
        },
      ],
    });
  
    humLine.value = Highcharts.chart("container0", {
      chart: { zoomType: "x" },
      title: { text: "Humidity Analysis", align: "left" },
      yAxis: {
        title: {
          text: "Humidity",
          style: { color: "#000000" },
        },
        labels: { format: "{value} %" },
      },
  
      tooltip: {
        pointFormat: "Humidity: {point.x} % ",
      },
      xAxis: {
        type: "datetime",
        title: { text: "Time", style: { color: "#000000" } },
      },
      tooltip: { shared: true },
      series: [
        {
          name: "Humidity",
          type: "line",
          data: [],
          turboThreshold: 0,
          color: Highcharts.getOptions().colors[0],
        },
      ],
    });
  
    histo.value = Highcharts.chart("container1", {
      chart: { zoomType: "x" },
      title: { text: "Frequency Distribution Analysis", align: "left" },
      yAxis: {
        title: {
          text: "Frequency",
          style: { color: "#000000" },
        },
        labels: { format: "{value}" },
      },
  
      xAxis: {
        title: { text: "ID", style: { color: "#000000" } },
      },
      tooltip: { shared: true },
      series: [
        {
          name: "Temperature",
          type: "column",
          data: [],
          turboThreshold: 0,
          color: Highcharts.getOptions().colors[0],
        },
        {
          name: "Humidity",
          type: "column",
          data: [],
          turboThreshold: 0,
          color: Highcharts.getOptions().colors[1],
        },
        {
          name: "Heat Index",
          type: "column",
          data: [],
          turboThreshold: 0,
          color: Highcharts.getOptions().colors[3],
        },
      ],
    });
  
    tempHiScat.value = Highcharts.chart("container2", {
      chart: { zoomType: "x" },
      title: {
        text: "Temperature & Heat Index Correlation Analysis",
        align: "left",
      },
      subtitle: {
        text: "Visualize the relationship between Temperature and Heat Index as well as revealing patterns or trends in the data",
      },
      yAxis: {
        title: {
          text: "Heat Index",
          style: { color: "#000000" },
        },
        labels: { format: "{value} °C" },
      },
  
      xAxis: {
        title: { text: "Temperature", style: { color: "#000000" } },
        labels: { format: "{value} °C" },
      },
      tooltip: {
        shared: true,
        pointFormat: "Temperature: {point.x} °C <br/> Heat Index: {point.y} °C",
      },
      series: [
        {
          name: "Analysis",
          type: "scatter",
          data: [],
          turboThreshold: 0,
          color: Highcharts.getOptions().colors[0],
        },
      ],
    });
  
    humScat.value = Highcharts.chart("container3", {
      chart: { zoomType: "x" },
      title: {
        text: "Humidity & Heat Index Correlation Analysis",
        align: "left",
      },
      subtitle: {
        text: "Visualize the relationship between Humidity and Heat Index as well as revealing patterns or trends in the data",
      },
      yAxis: {
        title: {
          text: "Heat Index",
          style: { color: "#000000" },
        },
        labels: { format: "{value} °C" },
      },
  
      xAxis: {
        title: { text: "Humidity", style: { color: "#000000" } },
        labels: { format: "{value} %" },
      },
      tooltip: {
        shared: true,
        pointFormat: "Humidity: {point.x} °C <br/> Heat Index: {point.y} °C",
      },
      series: [
        {
          name: "Analysis",
          type: "scatter",
          data: [],
          turboThreshold: 0,
          color: Highcharts.getOptions().colors[5],
        },
      ],
    });
  };
  
  onMounted(() => {
    // THIS FUNCTION IS CALLED AFTER THIS COMPONENT HAS BEEN MOUNTED
    Mqtt.connect(); // Connect to Broker located on the backend
    setTimeout(() => {
      // Subscribe to each topic
      Mqtt.subscribe("620152241");
      Mqtt.subscribe("620152241_sub");
    }, 3000);
    CreateCharts();
  });
  
  onBeforeUnmount(() => {
    // THIS FUNCTION IS CALLED RIGHT BEFORE THIS COMPONENT IS UNMOUNTED
    Mqtt.unsubcribeAll();
  });
  
  const updateLineCharts = async () => {
    if (!!start.value && !!end.value) {
      // Convert output from Textfield components to 10 digit timestamps
      let startDate = new Date(start.value).getTime() / 1000;
      let endDate = new Date(end.value).getTime() / 1000;
      // Fetch data from backend
      const data = await AppStore.getAllInRange(startDate, endDate);
      // Create arrays for each plot
      let temperature = [];
      let heatindex = [];
      let humidity = [];
      // Iterate through data variable and transform object to format recognized by highcharts
      data.forEach((row) => {
        temperature.push({
          x: row.timestamp * 1000,
          y: parseFloat(row.temperature.toFixed(2)),
        });
        heatindex.push({
          x: row.timestamp * 1000,
          y: parseFloat(row.heatindex.toFixed(2)),
        });
        humidity.push({
          x: row.timestamp * 1000,
          y: parseFloat(row.humidity.toFixed(2)),
        });
      });
      // Add data to Temperature and Heat Index chart
      tempHiLine.value.series[0].setData(temperature);
      tempHiLine.value.series[1].setData(heatindex);
      humLine.value.series[0].setData(humidity);
    }
  };
  
  const updateCards = async () => {
    // Retrieve Min, Max, Avg, Spread/Range
    if (!!start.value && !!end.value) {
      // 1. Convert start and end dates collected fron TextFields to 10 digit timestamps
      let startDate = new Date(start.value).getTime() / 1000;
      let endDate = new Date(end.value).getTime() / 1000;
      // 2. Fetch data from backend by calling the API functions
      const temp = await AppStore.getTemperatureMMAR(startDate, endDate);
      const humid = await AppStore.getHumidityMMAR(startDate, endDate);
      //console.log(temp);
      temperature.max = temp[0].max.toFixed(1);
      temperature.min = temp[0].min.toFixed(1);
      temperature.avg = temp[0].avg.toFixed(1);
      temperature.range = temp[0].range.toFixed(1);
      humidity.max = humid[0].max.toFixed(1);
      humidity.min = humid[0].min.toFixed(1);
      humidity.avg = humid[0].avg.toFixed(1);
      humidity.range = humid[0].range.toFixed(1);
    }
  };
  
  const updateHistogramCharts = async () => {
    // Retrieve Min, Max, Avg, Spread/Range for Column graph
    let startDate = new Date(start.value).getTime() / 1000;
    let endDate = new Date(end.value).getTime() / 1000;
    if (!!start.value && !!end.value) {
      const temp = await AppStore.getFreqDistro(
        "temperature",
        startDate,
        endDate
      );
      const humid = await AppStore.getFreqDistro("humidity", startDate, endDate);
      const hi = await AppStore.getFreqDistro("heatindex", startDate, endDate);
      // 3. create an empty array for each variable (temperature, humidity and heatindex)
      // see example below
      let temperature = [];
      let humidity = [];
      let heatindex = [];
      temp.forEach((row) => {
        temperature.push({ x: row["_id"], y: row["count"] });
      });
      humid.forEach((row) => {
        humidity.push({ x: row["_id"], y: row["count"] });
      });
      hi.forEach((row) => {
        heatindex.push({ x: row["_id"], y: row["count"] });
      });
      histo.value.series[0].setData(temperature);
      histo.value.series[1].setData(humidity);
      histo.value.series[2].setData(heatindex);
    }
  };
  
  const updateScatter = async () => {
    if (!!start.value && !!end.value) {
      // Convert output from Textfield components to 10 digit timestamps
      let startDate = new Date(start.value).getTime() / 1000;
      let endDate = new Date(end.value).getTime() / 1000;
      // Fetch data from backend
      const data = await AppStore.getAllInRange(startDate, endDate);
      // Create arrays for each plot
      let scatterPoints1 = [];
      let scatterPoints2 = [];
      // Iterate through data variable and transform object to format recognized by highcharts
      data.forEach((row) => {
        scatterPoints1.push({
          x: parseFloat(row.temperature.toFixed(2)),
          y: parseFloat(row.heatindex.toFixed(2)),
        });
      });
  
      data.forEach((row) => {
        scatterPoints2.push({
          x: parseFloat(row.humidity.toFixed(2)),
          y: parseFloat(row.heatindex.toFixed(2)),
        });
      });
      // Add data to Temperature and Heat Index chart
      tempHiScat.value.series[0].setData(scatterPoints1);
      humScat.value.series[0].setData(scatterPoints2);
    }
  };
  </script>
  
  <style scoped>
  /** CSS STYLE HERE */
  
  .container {
    background-color: #f5f5f5;
    width: 1200px;
  }
  
  .row {
    max-width: 1200px;
  }
  
  .row1 {
    max-width: 1200px;
    padding: 1;
  }
  
  .col1 {
    border: black;
  }
  
  .sheet {
    padding: 2;
    height: 250;
  }
  
  Figure {
    border: 2px solid black;
  }
  </style>
  
<!-- <template>
      <v-container fluid align=" center" color=" surface"  >
        <v-row class="row1" max-width="1200px" justify="center" align="center" padding="1">
            <v-col class="col col1" >
                <v-sheet class="sheeet" height="250">
                    <p>Enter date range for Analysis</p>
                    <v-divider></v-divider>
                    <v-text-field label="Start date" type="Date" density="compact" solo-inverted class="mr-5" max-width="300px" flat v-model="start"></v-text-field>
                    <v-text-field label="End date" type="Date" density="compact" solo-inverted class="mr-5" max-width="300px" flat v-model="end"></v-text-field>
                    
                    <v-spacer></v-spacer>
                    <VBtn @click="updateLineCharts(); updateCards(); updateHistogramCharts();updateScatter(); " text="Analyze" color="primary" tonal></VBtn>
                    </v-sheet> 
            </v-col>

            <v-col class="col col2" cols="3" align="center"> 
                <v-card title="Temperature" width="250" outlines color="primary" density="compact" rounded="lg" border>
                    <v-card-item class="mb-n5">
                        <v-chip-group class="d-flex flex-row justify-center" color="primaryContainer" flat>
                            <v-tooltip text="Min" location="start">
                                <template v-slot:activator="{props}">
                                    <v-chip v-bind="props">{{ temperature.min }}</v-chip>
                                </template>             
                            </v-tooltip>
                            <v-tooltip text="Range" location="top">
                                <template v-slot:activator="{props}">
                                    <v-chip v-bind="props">{{ temperature.range }}</v-chip>
                                </template>     
                            </v-tooltip>
                            <v-tooltip text="Max" location="end">
                                <template v-slot:activator="{props}">
                                    <v-chip v-bind="props">{{ temperature.max }}</v-chip>
                                </template>     
                            </v-tooltip>
                        </v-chip-group>
                    </v-card-item>
                    <v-card-item align="center">
                        <span class="text-h1 text-primary font-weight-bold">
                            {{ temperature.avg }}
                        </span>
                    </v-card-item>
                </v-card>
            </v-col>

        <v-col class="col col3" cols="3" align="center"> 
                        <v-card title="Humidity" width="250" outlines color="primary" density="compact" rounded="lg" border>
                            <v-card-item class="mb-n5">
                                <v-chip-group class="d-flex flex-row justify-center" color="primaryContainer" flat>
                                    <v-tooltip text="Min" location="start">
                                        <template v-slot:activator="{props}">
                                            <v-chip v-bind="props">{{ humidity.min }}</v-chip>
                                        </template>             
                                    </v-tooltip>
                                    <v-tooltip text="Range" location="top">
                                        <template v-slot:activator="{props}">
                                            <v-chip v-bind="props">{{ humidity.range }}</v-chip>
                                        </template>     
                                    </v-tooltip>
                                    <v-tooltip text="Max" location="end">
                                        <template v-slot:activator="{props}">
                                            <v-chip v-bind="props">{{ humidity.max }}</v-chip>
                                        </template>     
                                    </v-tooltip>
                                </v-chip-group>
                            </v-card-item>
                            <v-card-item align="center">
                                <span class="text-h1 text-primary font-weight-bold">
                                    {{ humidity.avg }}
                                </span>
                            </v-card-item>
                        </v-card>
                    </v-col>
        </v-row>

        <v-row max-width="1200px" justify="start" align="center">
            <v-col class="col col1" cols="12" align = 'center'>
                <figure class="highcharts-figure">
                    <div id="container"></div>
                </figure>
            </v-col>

            <v-col class="col col2" cols="12" align="center">
                <figure class="highcharts-figure">
                    <div id="container0"></div>
                </figure>
            </v-col>
        </v-row>

        <v-row max-width="1200px" justify="start" align="center">
            <v-col class="col col1" cols="12" align="center" style="border-right: 2px solid black;">
                <figure class="highcharts-figure">
                    <div id="container1"></div>
                </figure>
            </v-col>

            <v-col class="col col2" cols="12" align="center">
                <figure class="highcharts-figure">
                    <div id="container2"></div>
                </figure>
            </v-col>

            <v-col class="col col3" cols="12" align="center">
                <figure class="highcharts-figure">
                    <div id="container3"></div>
                </figure>
            </v-col>
        </v-row>
    </v-container>
</template>

<script setup>
/** JAVASCRIPT HERE */

// IMPORTS
import { ref,reactive,watch ,onMounted,onBeforeUnmount,computed } from "vue";  
import { useRoute ,useRouter } from "vue-router";
import { useAppStore } from "@/store/appStore";
import Highcharts from 'highcharts';
import more from 'highcharts/highcharts-more';
import { useMqttStore } from '@/store/mqttStore'; // Import Mqtt Store
import { storeToRefs } from "pinia";
import Exporting from 'highcharts/modules/exporting';
Exporting(Highcharts);
more(Highcharts);

// VARIABLES
const router      = useRouter();
const route       = useRoute();  
const Mqtt = useMqttStore();
const { payload, payloadTopic } = storeToRefs(Mqtt);
const AppStore = useAppStore();
const start = ref("");
const end = ref("");
const tempHiChart = ref(null); // Chart object
const humHiChart = ref(null); // Chart objects
const colHiChart = ref(null); // Chart object
const scatHiChart = ref(null); // Chart object
const scat2HiChart = ref(null); // Chart object

// FUNCTIONS
// Define reactive variables for temperature and humidity
const temperature = reactive({"min": 0,"range": 0,"max": 0, "avg":0});

const humidity = reactive({"min": 0,"range": 0,"max": 0, "avg": 0});

// FUNCTIONS
const CreateCharts = async () => {
    // TEMPERATURE CHART
    tempHiChart.value = Highcharts.chart('container', {
    chart: { zoomType: 'x' },
    title: { text: 'Air Temperature and Heat Index Analysis', align: 'left' },
    subtitle:{text:'The heat index, also known as the "apparent temperature," is a measure that combines air temperature and relative humidity to assess how hot it feels to the human body. The relationship between heat index and air temperature is influenced by humidity levels. As humidity increases, the heat index also rises, making the perceived temperature higher than the actual air temperature.', },
    yAxis: { 
        title: { text: 'Air Temperature & Heat Index' , style:{color:'#000000'}},
        labels: { format: '{value} °C' } 
    },
    xAxis: {
        type: 'datetime', 
        title: { text: 'Time', style:{color:'#000000'} },
    },
    tooltip: { shared:true, pointFormat: 'Heat Index: {point.x} % <br/> Temperature: {point.y} °C' 
        },
    series: [
        {
            name: 'Temperature',
            type: 'spline',
            data: [],
            turboThreshold: 0,
            color: Highcharts.getOptions().colors[0]
        }, 
        {
            name: 'Heat Index',
            type: 'spline',
            data: [],
            turboThreshold: 0,
            color: Highcharts.getOptions().colors[1]
        }],
});
    humHiChart.value=Highcharts.chart('container0', {
        chart: { zoomType: 'x' },
        title: { text: 'Humidity Analysis', align: 'left' },
        yAxis: { 
            title: { text: 'Humidity',style: { color: "#000000" }, },
            labels: { format: '{value} %' } 
        },
        xAxis: {
            type: 'datetime', 
            title: { text: 'Time', style: { color: "#000000"} },
        },
        tooltip: { 
            shared: true,
            pointFormat: 'Humidity: {point.x} %'},
        series: [
            // Series data for humidity
            {
            name: 'Humidity',
            type: 'line',
            data: [],
            turboThreshold: 0,
            color: Highcharts.getOptions().colors[0]
            },
        ],
    });

    colHiChart.value=Highcharts.chart('container1', {
        chart: { type: 'column', zoomType: 'x' },
        title: { text: 'Frequency Distribution Analysis', align: 'left' },
        // Add series data for Temperature, Humidity, and Heat Index variables
     yAxis: {
      title: {
        text: "Frequency",
        style: { color: "#000000" },
      },
      labels: { format: "{value}" },
    },

    xAxis: {
      title: { text: "ID", style: { color: "#000000" } },
    },
    tooltip: { shared: true },
        series: [
            // Series data for Temperature
            // Series data for Humidity
            // Series data for Heat Index
            {
            name: 'Temperature',
            type: 'bar',
            data: [],
            turboThreshold: 0,
            color: Highcharts.getOptions().colors[0]
        },  
         {
            name: 'Humidity',
            type: 'bar',
            data: [],
            turboThreshold: 0,
            color: Highcharts.getOptions().colors[0]
            },
        {
            name: 'Heat Index',
            type: 'bar',
            data: [],
            turboThreshold: 0,
            color: Highcharts.getOptions().colors[1]
        },
        ],
    });

    scatHiChart.value=Highcharts.chart('container2', {
        chart: { type: 'scatter', zoomType: 'x' },
        title: { text: 'Temperature & Heat Index Correlation Analysis', align: 'left' },
        subtitle: { text: 'Visualize the relationship between Temperature and Heat Index as well as revealing patterns or trends in the data' },
        xAxis: {
            title: { text: 'Temperature',style: { color: "#000000" }, },
            labels: { format: '{value} °C' }
        },
        yAxis: {
            title: { text: 'Heat Index', style: { color: "#000000" },},
            labels: { format: '{value} °C' }
        },
        tooltip: {
            pointFormat: 'Temperature: {point.x} °C <br/> Heat Index: {point.y} °C',
        },
        series: [
            // Series data for Temperature and Heat Index
            {
        name: "Analysis",
        type: "scatter",
        data: [],
        turboThreshold: 0,
        color: Highcharts.getOptions().colors[0],
        }
        ]
    });

    scat2HiChart.value =Highcharts.chart('container3', {
        chart: { type: 'scatter', zoomType: 'x' },
        title: { text: 'Humidity & Heat Index Correlation Analysis', align: 'left' },
        subtitle: { text: 'Visualize the relationship between Humidity and Heat Index as well as revealing patterns or trends in the data' },
        xAxis: {
            title: { text: 'Humidity', style: { color: "#000000" }},
            labels: { format: '{value} %' }
        },
        yAxis: {
            title: { text: 'Heat Index', style: { color: "#000000"} },
            labels: { format: '{value} °C' }
        },
        tooltip: {
            pointFormat: 'Humidity: {point.x} % <br/> Heat Index: {point.y} °C'
        },
        series: [
            // Series data for Humidity and Heat Index
            {
        name: "Analysis",
        type: "scatter",
        data: [],
        turboThreshold: 0,
        color: Highcharts.getOptions().colors[5],
        },
        ]
    });

};
onMounted(()=>{
    // THIS FUNCTION IS CALLED AFTER THIS COMPONENT HAS BEEN MOUNTED
    CreateCharts();

    Mqtt.connect(); // Connect to Broker located on the backend
   
    setTimeout( ()=>{
    // Subscribe to each topic
    Mqtt.subscribe("620152241");
    Mqtt.subscribe("620152241_sub");
    },3000)// THIS FUNCTION IS CALLED AFTER THIS COMPONENT HAS BEEN MOUNTED
});


onBeforeUnmount(()=>{
    // THIS FUNCTION IS CALLED RIGHT BEFORE THIS COMPONENT IS UNMOUNTED
    Mqtt.unsubcribeAll();
});

const updateLineCharts = async ()=>{
    if(!!start.value && !!end.value){
        // Convert output from Textfield components to 10 digit timestamps
        let startDate = new Date(start.value).getTime() / 1000;
        let endDate = new Date(end.value).getTime() / 1000;

        // Fetch data from backend
        const data = await AppStore.getAllInRange(startDate,endDate);

        // Create arrays for each plot
        let temperature = [];
        let heatindex = [];
        let humidity = [];

        // Iterate through data variable and transform object to format recognized by highcharts
        data.forEach(row => {
        temperature.push({x: row.timestamp * 1000, y: parseFloat(row.temperature.toFixed(2)) });
        heatindex.push({ x: row.timestamp * 1000,y: parseFloat(row.heatindex.toFixed(2)) });
        humidity.push({x: row.timestamp * 1000,y: parseFloat(row.humidity.toFixed(2)),
      });
    });

    // Add data to Temperature and Heat Index chart
    tempHiChart.value.series[0].setData(temperature);
    tempHiChart.value.series[1].setData(heatindex);
    humHiChart.value.series[0].setData(humidity);
    }
}

const updateCards = async () => {
    // Retrieve Min, Max, Avg, Spread/Range
    if(!!start.value && !!end.value){
    
        // 1. Convert start and end dates collected fron TextFields to 10 digit timestamps
        let startDate = new Date(start.value).getTime() / 1000;
        let endDate = new Date(end.value).getTime() / 1000;
        
        // 2. Fetch data from backend by calling the API functions
        const temp = await AppStore.getTemperatureMMAR(startDate,endDate);
        const humid = await AppStore.getHumidityMMAR(startDate,endDate);
        console.log(humid)
        
        temperature.max = temp[0].max.toFixed(1);
        //3. complete for min, avg and range
        temperature.min = temp[0].min.toFixed(1);
        temperature.avg = temp[0].avg.toFixed(1);
        temperature.range = temp[0].range.toFixed(1);
       
        //4. complete max, min, avg and range for the humidity variable
        humidity.max = temp[0].max.toFixed(1);
        humidity.min = temp[0].min.toFixed(1);
        humidity.avg = temp[0].avg.toFixed(1);
        humidity.range = temp[0].range.toFixed(1);
    }
}

const updateHistogramCharts = async () =>{
// Retrieve Min, Max, Avg, Spread/Range for Column graph
        let startDate = new Date(start.value).getTime() / 1000;
        let endDate = new Date(end.value).getTime() / 1000;

    if (!!start.value && !!end.value) {
        // 1. Convert start and end dates collected from TextFields to 10 digit timestamps
        // Subsequently, create startDate and endDate variables and then save the respective timestamps in these variables
        // const startDate = Date.parse(start.value) / 1000;
        // const endDate = Date.parse(end.value) / 1000;
       
        // 2. Fetch data (temp, humid, and hi) from the backend by calling the getFreqDistro API functions for each
        const temp = await AppStore.getFreqDistro("temperature", startDate, endDate);
        const humid = await AppStore.getFreqDistro("humidity", startDate, endDate);
        const hi = await AppStore.getFreqDistro("heatindex", startDate, endDate);
        
        // 3. Create an empty array for each variable (temperature, humidity, and heatindex)
        let temperature = [];
        let humidity = [];
        let heatindex = [];
        
        // 4. Iterate through the temp variable, which contains temperature data fetched from the backend
        // Transform the data to {"x": x_value, "y": y_value} format and then push it to the temperature array created previously
        temp.forEach(row => {
            temperature.push({ "x": row["_id"], "y": row["count"] });
        });
        
        // 5. Iterate through the humid variable, which contains humidity data fetched from the backend
        // Transform the data to {"x": x_value, "y": y_value} format and then push it to the humidity array created previously
        humid.forEach(row => {
            humidity.push({ "x": row["_id"], "y": row["count"] });
        });
        
        // 6. Iterate through the hi variable, which contains heat index data fetched from the backend
        // Transform the data to {"x": x_value, "y": y_value} format and then push it to the heatindex array created previously
        hi.forEach(row => {
            heatindex.push({ "x": row["_id"], "y": row["count"] });
        });
        
        // 7. Update series[0] for the histogram/Column chart with temperature data
        colHiChart.value.series[0].setData(temperature);
        
        // 8. Update series[1] for the histogram/Column chart with humidity data
        colHiChart.value.series[1].setData(humidity);
        
        // 9. Update series[2] for the histogram/Column chart with heat index data
        colHiChart.value.series[2].setData(heatindex);
    }
}

const updateScatter = async () => {
  if (!!start.value && !!end.value) {
    // Convert output from Textfield components to 10 digit timestamps
    let startDate = new Date(start.value).getTime() / 1000;
    let endDate = new Date(end.value).getTime() / 1000;
    // Fetch data from backend
    const data = await AppStore.getAllInRange(startDate, endDate);
    // Create arrays for each plot
    let scatterPoints1 = [];
    let scatterPoints2 = [];
    // Iterate through data variable and transform object to format recognized by highcharts
    data.forEach((row) => {
      scatterPoints1.push({
        x: parseFloat(row.temperature.toFixed(2)),
        y: parseFloat(row.heatindex.toFixed(2)),
      });
    });

    data.forEach((row) => {
      scatterPoints2.push({
        x: parseFloat(row.humidity.toFixed(2)),
        y: parseFloat(row.heatindex.toFixed(2)),
      });
    });
    // Add data to Temperature and Heat Index chart
    scatHiChart.value.series[0].setData(scatterPoints1);
    scat2HiChart.value.series[0].setData(scatterPoints2);
  }
};
</script>


<style scoped>
/** CSS STYLE HERE */
.container {
  background-color: #f5f5f5;
  width: 1200px;
}

.row {
  max-width: 1200px;
}

.row1 {
  max-width: 1200px;
  padding: 1;
}

.col1 {
  border: black;
}

.sheet {
  padding: 2;
  height: 250;
}

Figure {
  border: 2px solid black;
}

</style>
   -->