# Tell what image we are using
FROM dhi.io/deno:latest

# Create the work directory
WORKDIR /app

# Copy the deno.json, which includes the dependencies ne>
COPY deno.json .

# I am not sure what does DENO_FUTURE=1 do
# Install the dependencies
RUN DENO_FUTURE=1 deno install

# Copy everything else
COPY . .

# Set a port environment variable
ENV PORT=8000

# Expose the port
EXPOSE 8000

# Running the app-run.js file and allowing:
# - environment variable access
# - network access
# - watching for changes (reactivity)
CMD [ "run", "--allow-env", "--allow-net", "--watch", "app-run.json" ]





